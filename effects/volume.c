#include "volume.h"

CFUNCTION int effect_ampmax(slevel_t* samples, size_t length)
{
    if(samples == NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;

    slevel_t maxabs = samples_findmaxabs(samples, length);
    // Finding maximum absolute value from array of samples.

    if(maxabs == SLEVEL_MAX) return FUNC_OK;
    // If maximum sample level is SLEVEL_MAX, we don't need to
    // amplify the signal (multiply samples).

    double amp_ratio = ((double)SLEVEL_MAX/(double)maxabs);
    // Amplify ratio is the ratio of SLEVEL_MAX to maximum
    // absolute value (i.e factor, at which the modulo of
    // maximum sample will be equal SLEVEL_MAX).

    return effect_amplify(samples, length, amp_ratio);
}

CFUNCTION int effect_amplify(slevel_t* samples, size_t length, double multiplier)
{
    if(samples == NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;
    
    if(multiplier == 1) return FUNC_OK;

    #pragma omp parallel for schedule(static)
    for(omp_iter_t i = 0; i < length; i++)
    {
        if(samples[i] == 0) continue;

        double sample = samples[i] * multiplier;

        SLEVEL_CLIPPING(sample, samples[i]);
        // Samples after amplifying can be out of slevel_t range.
        // If sample is out of slevel_t range, it will be clipped.
    }

    return FUNC_OK;
}

CFUNCTION int effect_fade(slevel_t* samples, size_t length, fheader* header, double initvolume, size_t duration, bool reverse, interp_method method)
{
    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;

    if(initvolume == 1 || duration == 0) return FUNC_OK;

    uint64_t channels = header->channels;

    if(channels < 1 || length % channels)
    // Number of samples in each channel must be the same.
        return FUNC_INVALID_ARG;

    size_t loopend = duration*channels;
    if(loopend > length) return FUNC_INVALID_ARG;
    // If end of loop would be greater than length,
    // then we'd get segmentation fault.

    size_t lastsample = length-1;
    size_t lastchannel = channels-1;
    // These variables are used ONLY if reverse is true.

    #pragma omp parallel for schedule(static)
    for(omp_iter_t i = 0; i < loopend; i += channels)
    {
        double volume = math_interpolate(method, initvolume, 1.0, ((double)i/loopend));

        for(uint64_t ci = 0; ci < channels; ci++)
        {
            if(reverse)
            // Condition inside the loop is slower than outside the loop,
            // BUT it provides more code readability.
            {
                samples[(lastsample-i)-(lastchannel-ci)] *= volume;
                // We just reverse fade-in algorithm, beginning from last samples.
            }
            else samples[i+ci] *= volume;
        }
    }
    
    return FUNC_OK;
}

CFUNCTION int effect_clipping(slevel_t* samples, size_t length, slevel_t max, slevel_t min)
{
    if(samples == NULL) return FUNC_INVALID_ARG;
    if(max < 0 || min > 0) return FUNC_INVALID_ARG;

    if(max == SLEVEL_MAX && min == SLEVEL_MIN) return FUNC_OK;
    // Samples are already in range of [SLEVEL_MIN, SLEVEL_MAX],
    // so we can just don't clip them.

    #pragma omp parallel for schedule(static)
    for(omp_iter_t i = 0; i < length; i++)
    {
        if(samples[i] > max) samples[i] = max;
        else if(samples[i] < min) samples[i] = min;
        // Clipping effect is just limitation of levels.
    }

    return FUNC_OK;
}