#include "stretch.h"

CFUNCTION int effect_speedup(slevel_t** samples, size_t* length, fheader* header, size_t chunksize, size_t staying, interp_method fademethod)
{
    if(samples == NULL || (*samples) == NULL) return FUNC_INVALID_ARG;
    if(length == NULL || (*length) < 1) return FUNC_INVALID_ARG;
    if(header == NULL) return FUNC_INVALID_ARG;
    if(chunksize < 1 || staying < 1 || staying > chunksize) return FUNC_INVALID_ARG;

    size_t oglength = (*length);
    slevel_t* ogsamples = (*samples);
    // 'og' stands for 'original'

    uint64_t channels = header->channels;
    if(oglength < chunksize || channels < 1 || oglength % channels) return FUNC_INVALID_ARG;
    // Length must be multiple of channels, because
    // number of samples in each channel must be the same.

    size_t unclaimed = chunksize - staying;
    // This is number of samples,
    // that won't be moved into result array.

    if(staying < unclaimed) return FUNC_INVALID_ARG;
    // Algorithm based on that number of unclaimed samples
    // in the chunk is less than or equals number of staying samples.

    double* fading_lut = (double*)malloc(unclaimed * sizeof(double));
    // We're creating fading lookup table (LUT)
    // Without it, we'd calculate fading in each iteration of the loop.

    if(fading_lut == NULL) return FUNC_MEMALLOC_FAILED;

    for(omp_iter_t i = 0; i < unclaimed; i++)
    {
        fading_lut[i] = math_interpolate(fademethod, 0, 1, ((double)i/unclaimed));
        // We make fading with interpolation function.
    }

    oglength -= oglength % (chunksize*channels);
    // We subtract part of samples that less
    // than one chunk in all channels, to avoid
    // segmentation faults.

    size_t newlength = (oglength/chunksize)*staying;

    slevel_t* result = (slevel_t*)malloc(newlength * sizeof(slevel_t));
    if(result == NULL) return FUNC_MEMALLOC_FAILED;
    // If allocation was failed, malloc will return NULL pointer

    size_t chaunclaimed = unclaimed*channels;
    size_t chastaying = staying*channels;
    size_t chachunksize = chunksize*channels;
    // 'cha' stands for channels.
    // Number of samples in chunk is for one channel,
    // so if we need to apply effect to all channels,
    // (because of channel interleaving) we have to
    // multiply chunksize, staying, and unclaimed
    // by number of channels.

    #pragma omp parallel for schedule(static)
    for(omp_iter_t i = 0; i < oglength; i += chachunksize)
    {
        size_t newi = ((i/chunksize)*staying);
        // Iterator for result array.

        for(size_t ri = 0; ri < chastaying; ri++)
        {
            result[newi+ri] = ogsamples[i+ri];
        }
        // Collecting remaining samples.

        for(size_t ri = chastaying; ri < chachunksize; ri++)
        {
            double fading = fading_lut[(ri/channels)-staying];

            result[newi+(ri-chaunclaimed)] *= (1.0-fading);
            result[newi+(ri-chaunclaimed)] += (ogsamples[i+ri]*fading);
        }
        // Cross-fading with rest of the samples (unclaimed samples) to
        // avoid sudden level jumps.
    }

    free(fading_lut);
    // Freeing memory, occupied by lookup table.

    free(*samples);
    // Freeing original samples array.

    (*samples) = result;
    (*length) = newlength;

    return FUNC_OK;
}