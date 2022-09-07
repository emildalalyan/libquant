#include "volume.h"

CFUNCTION int effect_ampmax(slevel_t* samples, size_t length)
{
    if(samples == NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;

    slevel_t maxabs = samples_findmaxabs(samples, length);

    if(errno != 0) return FUNC_INTERNAL_ERROR;
    // samples_findmaxabs() sets errno to EINVAL if invalid
    // arguments were passed, and to 0, if arguments were right.

    if(maxabs == SLEVEL_MAX) return FUNC_OK;
    // If maximum sample level is SLEVEL_MAX, we don't need to
    // amplify the signal (multiply samples).

    double amp_ratio = ((double)SLEVEL_MAX/(double)maxabs);

    return effect_amplify(samples, length, amp_ratio);
}

CFUNCTION int effect_amplify(slevel_t* samples, size_t length, double ratio)
{
    if(samples == NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;
    
    if(ratio == 1) return FUNC_OK;

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < length; i++)
    {
        if(samples[i] == 0) continue;

        double sample = samples[i] * ratio;

        if(sample < SLEVEL_MIN) samples[i] = SLEVEL_MIN;
        else if(sample > SLEVEL_MAX) samples[i] = SLEVEL_MAX;
        else samples[i] = (slevel_t)sample;
        // Samples after amplifying can be out of slevel_t range.
        // And second reason is floating-point numbers. They are inaccurate,
        // so we have to check whether it's out of slevel_t range or not.
        // If sample is out of slevel_t range, it will be clipped,
        // i.e it will set to SLEVEL_MAX or SLEVEL_MIN for positive and negative
        // sample level respectively.
    }

    return FUNC_OK;
}

CFUNCTION int effect_fadein(slevel_t* samples, size_t length, fheader* header, double initvolume, size_t duration)
{
    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;

    if(initvolume == 1 || duration == 0) return FUNC_OK;

    uint64_t channels = header->channels;

    if(channels < 1 || length % channels)
    // Length must be multiple of channels, because
    // number of samples in each channel must be the same.
        return FUNC_INVALID_ARG;

    size_t loopend = duration*channels;
    if(loopend > length) return FUNC_INVALID_ARG;
    // If end of loop would be greater than length,
    // then we'd get segmentation fault.

    double step = (1-initvolume)/loopend;

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < loopend; i += channels)
    {
        double volume = (initvolume+(i*step));
        
        for(uint64_t ci = 0; ci < channels; ci++)
        {
            samples[i+ci] *= volume;
        }
    }

    return FUNC_OK;
}

CFUNCTION int effect_fadeout(slevel_t* samples, size_t length, fheader* header, double finalvolume, size_t duration)
{
    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;

    if(finalvolume == 1 || duration == 0) return FUNC_OK;

    uint64_t channels = header->channels;

    if(channels < 1 || length % channels)
    // Length must be multiple of channels, because
    // number of samples in each channel must be the same.
        return FUNC_INVALID_ARG;

    size_t loopend = duration*channels;
    if(loopend > length) return FUNC_INVALID_ARG;
    // If end of loop would be greater than length,
    // then we'd get segmentation fault.

    double step = (1-finalvolume)/loopend;

    size_t lastsample = length-1;
    size_t lastchannel = channels-1;

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < loopend; i += channels)
    {
        double volume = (finalvolume+(i*step));

        for(uint64_t ci = 0; ci < channels; ci++)
        {
            samples[(lastsample-i)-(lastchannel-ci)] *= volume;
            // We just reverse fade-in algorithm, beginning from last samples.
        }
    }

    return FUNC_OK;
}

CFUNCTION int effect_distort(slevel_t* samples, size_t length, slevel_t max, slevel_t min)
{
    if(samples == NULL) return FUNC_INVALID_ARG;
    if(max < 0 || min > 0) return FUNC_INVALID_ARG;

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < length; i++)
    {
        if(samples[i] > max) samples[i] = max;
        else if(samples[i] < min) samples[i] = min;
        // Clipping effect is just limitation of levels.
    }

    return FUNC_OK;
}