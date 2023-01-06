#include "stretch.h"

CFUNCTION int effect_speedup(slevel_t** samples, size_t* length, fheader* header, size_t chunksize, size_t remaining, interp_method fademethod)
{
    /* Arguments processing ============= */
    
    if(samples == NULL || (*samples) == NULL) return FUNC_INVALID_ARG;
    if(length == NULL || (*length) < 1) return FUNC_INVALID_ARG;
    if(header == NULL) return FUNC_INVALID_ARG;
    if(chunksize < 1 || remaining < 1) return FUNC_INVALID_ARG;

    size_t oglength = (*length);
    slevel_t* ogsamples = (*samples);

    if(remaining > chunksize || oglength < chunksize) return FUNC_INVALID_ARG;

    uint64_t channels = header->channels;
    if(channels < 1 || oglength % channels) return FUNC_INVALID_ARG;
    // Number of samples in each channel must be the same.

    size_t unclaimed = chunksize - remaining;
    // Number of samples, that won't be moved into resulting array.

    size_t chaunclaimed = unclaimed*channels;
    size_t charemaining = remaining*channels;
    size_t chachunksize = chunksize*channels;
    // To produce effect for multiple channels, we have to
    // multiply these variables by number of channels.
    
    oglength -= oglength % chachunksize;
    // We subtract part of samples that less
    // than one chunk in all channels, to avoid
    // segmentation faults.

    size_t fadingsize = fmin(remaining, unclaimed);
    size_t chafadingstart = chachunksize-(fadingsize*channels);

    /* ================================== */

    /* Fading Lookup table ============== */

    double* fading_lut = (double*)malloc(fadingsize * sizeof(double));
    if(fading_lut == NULL) return FUNC_MEMALLOC_FAILED;
    // Without it, we'd calculate fading in each iteration of the loop.

    for(size_t i = 0; i < fadingsize; i++)
        fading_lut[i] = math_interpolate(fademethod, 0, 1, ((double)i/fadingsize));

    /* ================================== */

    /* Allocating resulting array ======= */

    size_t newlength = (oglength/chunksize)*remaining;

    slevel_t* result = (slevel_t*)malloc(newlength * sizeof(slevel_t));
    if(result == NULL) return FUNC_MEMALLOC_FAILED;

    /* ================================== */

    /* Doing speed-up =================== */

    #pragma omp parallel for schedule(static)
    for(omp_iter_t i = 0; i < oglength; i += chachunksize)
    {
        size_t resi = ((i/chachunksize)*charemaining);
        // Iterator for resulting array.

        memcpy(&result[resi], &ogsamples[i], charemaining * sizeof(slevel_t));
        // Moving remaining samples.

        for(size_t fi = chafadingstart; fi < chachunksize; fi++)
        {
            double volume = fading_lut[((fi-chafadingstart)/channels)];

            result[resi+(fi-chaunclaimed)] *= (1.0-volume);
            result[resi+(fi-chaunclaimed)] += (ogsamples[i+fi]*volume);
        }
        // Cross-fading with unclaimed samples to avoid level jumps.
    }
    
    /* ================================== */

    /* Freeing memory and assignment ==== */

    free(fading_lut);
    free(*samples);

    (*samples) = result;
    (*length) = newlength;

    /* ================================== */

    return FUNC_OK;
}