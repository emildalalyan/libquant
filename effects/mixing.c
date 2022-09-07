#include "mixing.h"

CFUNCTION int effect_chamixdown(slevel_t** samples, fheader* header, size_t* length, uint64_t newchannels)
{
    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(newchannels < 1) return FUNC_INVALID_ARG;
    if(length == NULL || (*length) < 1) return FUNC_INVALID_ARG;

    size_t oglength = *length;
    uint64_t ogchannels = header->channels;
    slevel_t* ogsamples = *samples;
    // "OG" stands for "Original"

    if(ogchannels < 1 || newchannels >= ogchannels) return FUNC_INVALID_ARG;

    if(oglength % ogchannels) return FUNC_INVALID_ARG;
    // Length must be multiple of channels, because
    // number of samples in each channel must be the same.

    size_t newlength = (oglength/ogchannels) * newchannels;

    slevel_t* newsamples = (slevel_t*)malloc(newlength * sizeof(slevel_t));
    if(newsamples == NULL) return FUNC_MEMALLOC_FAILED;
    // If allocation was failed, malloc will return NULL pointer

    uint64_t restchannels = ogchannels - newchannels;

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < oglength; i += ogchannels)
    {
        double_t am_samples = 0;
        // 'double_t' is most efficient floating-point
        // type, at least as wide as double. 
        // This number contains arithmetic mean (AM)
        // between the rest of channels and 0.

        for(uint64_t ci = newchannels; ci < ogchannels; ci++)
        {
            am_samples += (ogsamples[i+ci])/(double_t)(restchannels+1);
        }

        for(uint64_t ci = 0; ci < newchannels; ci++)
        {
            size_t newi = ((i/ogchannels)*newchannels);

            newsamples[newi+ci] = (ogsamples[i+ci])/(double_t)(restchannels+1);
            newsamples[newi+ci] += am_samples;
        }
    }

    header->channels = newchannels;
    *length = newlength;

    free((*samples));
    *samples = newsamples;

    return FUNC_OK;
}

CFUNCTION int effect_chadiff(slevel_t** samples, size_t* length, fheader* header)
{
    if(samples == NULL || (*samples) == NULL || header == NULL) return FUNC_INVALID_ARG;

    slevel_t* ogsamples = *samples;
    size_t oglength = *length;
    uint64_t ogchannels = header->channels;
    // 'og' stands for 'original'

    if(ogchannels < 1 || oglength % ogchannels) return FUNC_INVALID_ARG;
    // oglength MUST BE multiple of ogchannels,
    // because number of samples in each channel must be the same.

    size_t newlength = (oglength/ogchannels);
    slevel_t* result = (slevel_t*)malloc(newlength * sizeof(slevel_t));
    
    if(result == NULL) return FUNC_MEMALLOC_FAILED;
    // If allocation was failed, malloc will return NULL pointer

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < oglength; i += ogchannels)
    {
        double_t arith_mean = 0;
        // 'double_t' is most efficient floating-point
        // type, at least as wide as double.
        // This contains arithmetic mean (AM)
        // between samples of each channel.

        for(uint64_t ci = 0; ci < ogchannels; ci++)
        {
            arith_mean += (ogsamples[i+ci]/(double_t)ogchannels);
        }
        
        size_t ri = i/ogchannels;
        // Iterator for result array.

        result[ri] = ogsamples[i+0] - arith_mean;
        // Difference between samples of each channel and
        // arithmetic mean of these channels are the same, but
        // in even channels, it's inverted. So we can subtract
        // arithmetic mean from first channel, and write result in mono.
    }

    free((*samples));
    (*samples) = result;
    // Freeing original samples and setting pointer to new array.

    *length = newlength;
    // Changing the length

    return FUNC_OK;
}