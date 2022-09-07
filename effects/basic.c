#include "basic.h"

CFUNCTION int effect_reverse(slevel_t* samples, fheader* header, size_t length)
{
    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    
    uint64_t channels = header->channels;

    if(channels < 1 || length % channels) return FUNC_INVALID_ARG;
    // Length must be multiple of channels, because
    // number of samples in each channel must be the same.

    slevel_t* result = (slevel_t*)malloc(length * sizeof(slevel_t));
    if(result == NULL) return FUNC_MEMALLOC_FAILED;
    // If allocation was failed, malloc will return NULL pointer

    size_t lastsample = length - 1;
    uint64_t lastchannel = channels - 1;

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < length; i += channels)
    {
        for(uint64_t ci = 0; ci < channels; ci++)
        {
            result[i+ci] = samples[(lastsample-i)-(lastchannel-ci)];
        }
    }

    memcpy(samples, result, length * sizeof(slevel_t));
    free(result);
    // Copying the result in provided array and freeing the memory.

    return FUNC_OK;
}

CFUNCTION int effect_invert(slevel_t* samples, size_t length)
{
    if(samples == NULL || length < 1) return FUNC_INVALID_ARG;

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < length; i++)
    {
        if(samples[i] == SLEVEL_MIN) samples[i] = SLEVEL_MAX;
        // On two's complement systems, -SLEVEL_MIN causes integer overflow.
        else samples[i] = -samples[i];
        // Inverting is just changing the sign.
    }

    return FUNC_OK;
}

CFUNCTION int effect_mute(slevel_t* samples, size_t length)
{
    if(samples == NULL || length < 1) return FUNC_INVALID_ARG;

    memset(samples, 0, length * sizeof(slevel_t));

    return FUNC_OK;
}

CFUNCTION int effect_mixnoise(slevel_t* samples, size_t length, double noisevolume)
{
    if(samples == NULL || length < 1) return FUNC_INVALID_ARG;

    if(noisevolume == 0) return FUNC_OK;
    // If volume of the noise is 0, then noise will be muted.

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < length; i++)
    {
        double rndnum = (rand()-(RAND_MAX/2.0))*(SLEVEL_MAX/(RAND_MAX/2.0));
        // We use (RAND_MAX/2.0), and not (RAND_MAX/2), for implicit
        // conversion to double-precision floating-point
        // number, because results can be fractional.
        // rand() produces pseudorandom number in the range 0 to RAND_MAX,
        // so we're shifting this range to -RAND_MAX/2 to RAND_MAX/2
        // to reduce clipping effect.

        double mixed = (double)samples[i] + (rndnum * noisevolume);

        if(mixed > SLEVEL_MAX) samples[i] = SLEVEL_MAX;
        else if(mixed < SLEVEL_MIN) samples[i] = SLEVEL_MIN;
        else samples[i] = mixed;
        // Floating-point numbers are inaccurate, so we have to check
        // whether it's out of slevel_t range, or not.
        // Second reason is noise level, it can be large, so we should clip
        // levels to avoid integer overflows.
    }

    return FUNC_OK;
}

CFUNCTION int effect_selectchannel(slevel_t** samples, size_t* length, fheader* header, uint64_t channelnum)
{
    if(samples == NULL || (*samples) == NULL || length == NULL || header == NULL)
        return FUNC_INVALID_ARG;

    uint64_t ogchannels = header->channels;
    slevel_t* ogsamples = (*samples);
    size_t oglength = (*length);
    // "og" stands for "original"

    if(ogchannels < 1 || channelnum >= ogchannels) return FUNC_INVALID_ARG;
    if(oglength % ogchannels) return FUNC_INVALID_ARG;
    // Length must be multiple of channels, because
    // number of samples in each channel must be the same.

    size_t newlength = (oglength/ogchannels);

    slevel_t* result = (slevel_t*)malloc(newlength * sizeof(slevel_t));
    if(result == NULL) return FUNC_MEMALLOC_FAILED;
    // If allocation was failed, malloc will return NULL pointer

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < newlength; i++)
    {
        result[i] = ogsamples[(i*ogchannels)+channelnum];
        // We need to multiply i by ogchannels,
        // because channels are interleaved in samples array.
    }

    free((*samples));
    (*samples) = result;

    return FUNC_OK;
}