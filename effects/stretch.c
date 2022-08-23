#include "stretch.h"

CFUNCTION int effect_speedup(slevel_t** samples, size_t* length, fheader* header, size_t chunksize, size_t staying)
{
    if(samples == NULL || (*samples) == NULL) return FUNC_INVALID_ARG;
    if(length == NULL || (*length) < 1) return FUNC_INVALID_ARG;
    if(header == NULL) return FUNC_INVALID_ARG;
    if(chunksize < 1 || staying < 1 || staying > chunksize) return FUNC_INVALID_ARG;

    size_t oglength = (*length);
    slevel_t* ogsamples = (*samples);

    uint64_t channels = header->channels;

    if(oglength < chunksize || channels < 1 || oglength % channels) return FUNC_INVALID_ARG;
    // Length must be multiple of channels, because
    // number of samples in each channel must be the same.

    size_t unclaimed = chunksize - staying;
    // This is number of samples,
    // that won't be moved into result array.

    if(staying < unclaimed) return FUNC_INVALID_ARG;

    oglength -= oglength % (chunksize*channels);
    // We subtract part of samples that less
    // than one chunk in all channels, to avoid
    // segmentation faults.

    size_t newlength = (oglength/chunksize)*staying;

    slevel_t* result = (slevel_t*)malloc(newlength * sizeof(slevel_t));
    if(result == NULL) return FUNC_INTERNAL_ERROR;
    // If allocation was failed, NULL
    // pointer will be returned by malloc.

    size_t chaunclaimed = unclaimed*channels;
    size_t chastaying = staying*channels;
    size_t chachunk = chunksize*channels;
    // 'cha' stands for channels.
    // Number of samples in chunk is for one channel,
    // so if we need to apply effect to all channels,
    // (because of channel interleaving) we have to
    // multiply chunk, staying, and unclaimed
    // by number of channels.

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < oglength; i += chachunk)
    {
        size_t newi = ((i/chunksize)*staying);
        // Iterator for result array.

        for(size_t ri = 0; ri < chastaying; ri++)
        {
            result[newi+ri] = ogsamples[i+ri];
        }
        // Collecting remaining samples.

        for(size_t ri = chastaying; ri < chachunk; ri++)
        {
            double_t fading = ((ri-chastaying)/(double_t)chaunclaimed);

            result[newi+(ri-chaunclaimed)] *= (1.0-fading);
            result[newi+(ri-chaunclaimed)] += (ogsamples[i+ri]*fading);
        }
        // Cross-fading with rest of the samples (unclaimed samples) to
        // avoid non-linear distortions.
    }

    free(*samples);
    (*samples) = result;
    (*length) = newlength;

    return FUNC_OK;
}