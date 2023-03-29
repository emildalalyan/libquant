#include "mixing.h"

CFUNCTION int effect_chamixdown(slevel_t** samples, fheader* header, size_t* length, uint32_t newchannels)
{
    /* Arguments processing ============= */
    
    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(newchannels < 1) return FUNC_INVALID_ARG;
    if(length == NULL || (*length) < 1) return FUNC_INVALID_ARG;

    size_t oglength = *length;
    uint32_t ogchannels = header->channels;
    slevel_t* ogsamples = *samples;

    if(ogchannels < 1 || newchannels >= ogchannels) return FUNC_INVALID_ARG;
    if(oglength % ogchannels) return FUNC_INVALID_ARG;
    // Number of samples in each channel must be the same.

    size_t newlength = (oglength/ogchannels)*newchannels;
    
    uint32_t removingchannels = ogchannels - newchannels;
    // Number of channels, that will be removed from the signal.

    /* ================================== */

    /* Mixing the channels ============== */

    slevel_t* newsamples = (slevel_t*)malloc(newlength * sizeof(slevel_t));
    if(newsamples == NULL) return FUNC_MEMALLOC_FAILED;

    #pragma omp parallel for schedule(static)
    for(omp_iter_t i = 0; i < oglength; i += ogchannels)
    {
        double_t armean = 0;
        // It containts arithmetic mean
        // of the removing channels and 0.

        for(uint32_t ci = newchannels; ci < ogchannels; ci++)
        {
            armean += (ogsamples[i+ci])/(double_t)(removingchannels+1);
        }
        // Calculating arithmetic mean of removing channels.

        for(uint32_t ci = 0; ci < newchannels; ci++)
        {
            size_t newi = ((i/ogchannels)*newchannels);

            newsamples[newi+ci] = (ogsamples[i+ci])/(double_t)(removingchannels+1);
            newsamples[newi+ci] += armean;
        }
        // Mixing this arithmetic mean with all remaining channels.
    }
    
    /* ================================== */

    /* Freeing memory and assignment ==== */

    header->channels = newchannels;
    *length = newlength;

    free((*samples));
    *samples = newsamples;
    
    /* ================================== */

    return FUNC_OK;
}

CFUNCTION int effect_extractchadiff(slevel_t* samples, size_t length, fheader* header)
{
    /* Arguments processing ============= */

    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;

    uint32_t channels = header->channels;
    if(channels < 1 || length % channels) return FUNC_INVALID_ARG;
    // Number of samples in each channel must be the same.
    
    /* ================================== */

    /* Extracting difference ============ */

    #pragma omp parallel for schedule(static)
    for(omp_iter_t i = 0; i < length; i += channels)
    {
        double_t armean = 0;
        for(uint32_t ci = 0; ci < channels; ci++)
        {
            armean += (samples[i+ci]/(double_t)channels);
        }
        // Calculating arithmetic mean.
        
        for(uint32_t ci = 0; ci < channels; ci++)
        {
            samples[i+ci] -= armean;
        }
        // Subtracting arithmetic mean from
        // samples of each channel.
    }

    /* ================================== */

    return FUNC_OK;
}

CFUNCTION int effect_amplifychadiff(slevel_t* samples, size_t length, fheader* header, double multiplier)
{
    /* Arguments processing ============= */

    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;
    if(multiplier < 0) return FUNC_INVALID_ARG;

    uint32_t channels = header->channels;
    if(channels < 1 || length % channels) return FUNC_INVALID_ARG;
    // Number of samples in each channel must be the same.
    
    /* ================================== */

    /* Amplifying difference ============ */

    #pragma omp parallel for schedule(static)
    for(omp_iter_t i = 0; i < length; i += channels)
    {
        double_t armean = 0;
        for(uint32_t ci = 0; ci < channels; ci++)
        {
            armean += (samples[i+ci]/(double_t)channels);
        }
        // Calculating arithmetic mean.

        for(uint32_t ci = 0; ci < channels; ci++)
        {
            double sample = samples[i+ci]*multiplier + armean*(1.0-multiplier);

            SLEVEL_CLIPPING(sample, samples[i+ci]);
        }
    }
    
    /* ================================== */

    return FUNC_OK;
}