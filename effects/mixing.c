#include "mixing.h"

CFUNCTION int effect_chamixdown(slevel_t** samples, fheader* header, size_t* length, uint64_t newchannels)
{
    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(newchannels < 1) return FUNC_INVALID_ARG;
    if(length == NULL || (*length) < 1) return FUNC_INVALID_ARG;

    size_t oglength = *length;
    uint64_t ogchannels = header->channels;
    slevel_t* ogsamples = *samples;
    // "og" stands for "original".

    if(ogchannels < 1 || newchannels >= ogchannels) return FUNC_INVALID_ARG;

    if(oglength % ogchannels) return FUNC_INVALID_ARG;
    // Length must be multiple of channels, because
    // number of samples in each channel must be the same.

    size_t newlength = (oglength/ogchannels)*newchannels;

    slevel_t* newsamples = (slevel_t*)malloc(newlength * sizeof(slevel_t));
    if(newsamples == NULL) return FUNC_MEMALLOC_FAILED;
    // If allocation was failed, malloc will return NULL pointer

    uint64_t removingchannels = ogchannels - newchannels;
    // Number of channels, that will be removed from the signal.

    #pragma omp parallel for schedule(static)
    for(omp_iter_t i = 0; i < oglength; i += ogchannels)
    {
        double_t amean = 0;
        // 'double_t' is most efficient floating-point
        // type, at least as wide as double. 
        // This number contains arithmetic mean (AM)
        // of the removing channels and 0.

        for(uint64_t ci = newchannels; ci < ogchannels; ci++)
        {
            amean += (ogsamples[i+ci])/(double_t)(removingchannels+1);
        }
        // Calculating arithmetic mean of removing channels.

        for(uint64_t ci = 0; ci < newchannels; ci++)
        {
            size_t newi = ((i/ogchannels)*newchannels);

            newsamples[newi+ci] = (ogsamples[i+ci])/(double_t)(removingchannels+1);
            newsamples[newi+ci] += amean;
        }
        // Mixing this arithmetic mean with all remaining channels.
    }

    header->channels = newchannels;
    *length = newlength;

    free((*samples));
    *samples = newsamples;

    return FUNC_OK;
}

CFUNCTION int effect_chadiff(slevel_t* samples, size_t length, fheader* header)
{
    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;

    uint64_t channels = header->channels;

    if(channels < 1 || length % channels) return FUNC_INVALID_ARG;
    // oglength MUST BE multiple of ogchannels,
    // because number of samples in each channel must be the same.

    #pragma omp parallel for schedule(static)
    for(omp_iter_t i = 0; i < length; i += channels)
    {
        double_t amean = 0;
        // 'double_t' is most efficient floating-point
        // type, at least as wide as double.
        // This contains arithmetic mean (AM)
        // between samples of each channel.

        for(uint64_t ci = 0; ci < channels; ci++)
        {
            amean += (samples[i+ci]/(double_t)channels);
        }
        // Calculating arithmetic mean between
        // samples of each channel.
        
        for(uint64_t ci = 0; ci < channels; ci++)
        {
            samples[i+ci] -= amean;
        }
        // Subtracting arithmetic mean from
        // samples of each channel.
    }

    return FUNC_OK;
}