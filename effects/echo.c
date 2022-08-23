#include "echo.h"

CFUNCTION int effect_echo(slevel_t* samples, size_t length, fheader* header, size_t delay, double feedback, size_t bounces)
{
    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(bounces < 1 || delay < 1 || length < 1) return FUNC_INVALID_ARG;

    if(feedback == 0) return FUNC_OK;
    // If feedback of surface is 0, then there's no echo.

    if(feedback < 0 || feedback > 1) return FUNC_INVALID_ARG;

    uint64_t channels = header->channels;

    if(channels < 1 || length % channels) return FUNC_INVALID_ARG;
    // Length must be multiple of channels, because
    // number of samples in each channel must be the same.

    if(delay > (length/channels)) return FUNC_OK;
    // If delay is greater than length of one channel, then audio
    // signal will end before we would hear a delayed signal,
    // so we won't hear it.

    slevel_t* result = (slevel_t*)malloc(length * sizeof(slevel_t));
    if(result == NULL) return FUNC_MEMALLOC_FAILED;
    // If allocation was failed, NULL
    // pointer will be returned by malloc

    double* feedb_lut = (double*)malloc(bounces * sizeof(double));
    // We're creating feedbacks lookup table (LUT)
    // Without it, we'd calculate feedbacks in each iteration of the loop.

    if(feedb_lut == NULL) return FUNC_MEMALLOC_FAILED;

    feedb_lut[0] = feedback;
    for(size_t i = 1; i < bounces; i++)
    {
        feedb_lut[i] = feedb_lut[i-1] * feedback;
        // If we'd call "pow" to exponentiate feedback,
        // it would slow, because of external function call process.
    }

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < length; i++)
    {
        double_t sample = samples[i];
        // 'double_t' is most efficient floating-point
        // type, at least as wide as double. 

        for(size_t si = 1; si <= bounces; si++)
        {
            size_t shift = (delay*channels)*si;

            if(i < shift) break;

            sample += (samples[i-shift] * feedb_lut[si-1]);
            // si counter starts with 1, and feedbacks lookup table
            // array starts with 0, so we subtract 1 from counter.
        }

        if(sample > SLEVEL_MAX) result[i] = SLEVEL_MAX;
        else if(sample < SLEVEL_MIN) result[i] = SLEVEL_MIN;
        else result[i] = sample;
        // Samples with applied echo can be out of slevel_t range,
        // because echo can be too loud.
        // And second reason is floating-point numbers. They are inaccurate,
        // so we have to check whether it's out of slevel_t range or not.
        // If sample is out of slevel_t range, it will be clipped,
        // i.e it will set to SLEVEL_MAX or SLEVEL_MIN for positive and negative
        // sample level respectively.
    }

    memcpy(samples, result, (length * sizeof(slevel_t)));
    free(result);

    free(feedb_lut);
    // We're freeing previously created lookup table array.

    return FUNC_OK;
}

CFUNCTION int effect_delay(slevel_t* samples, size_t length, fheader* header, size_t delay, double volume)
{
    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;

    if(volume == 0) return FUNC_OK;
    // If volume = 0, then delayed sound will be
    // muted, so the samples won't change.

    uint64_t channels = header->channels;

    if(channels < 1 || length < channels || length % channels) return FUNC_INVALID_ARG;
    // Length must be multiple of channels, because
    // number of samples in each channel must be the same.

    if(delay > (length/channels)) return FUNC_OK;
    // If delay is greater than length of one channel, then audio
    // signal will end before we would hear a delayed signal,
    // so we won't hear it.

    size_t shift = delay*channels;
    // Samples in array are interleaved by channels (i.e. they storage like as follows:
    // 1st sample of 1st channel, 1st sample of 2nd channel, 2nd sample of 1nd channel,
    // 2nd sample of 2nd channel, and so on), so we need to multiply delay
    // by channels to get correct shift.

    slevel_t* result = (slevel_t*)malloc(length * sizeof(slevel_t));
    if(result == NULL) return FUNC_MEMALLOC_FAILED;
    // If allocation was failed, NULL
    // pointer will be returned by malloc

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < length; i++)
    {
        if(i < shift)
        {
            result[i] = samples[i];
            continue;
        }

        double mixed = (double)samples[i] + (double)(samples[i-shift] * volume);

        if(mixed > SLEVEL_MAX) result[i] = SLEVEL_MAX;
        else if(mixed < SLEVEL_MIN) result[i] = SLEVEL_MIN;
        else result[i] = (slevel_t)mixed;
        // Floating point numbers are inaccurate, so we
        // have to check whether it's out of slevel_t range or not.
    }

    memcpy(samples, result, (length * sizeof(slevel_t)));
    free(result);

    return FUNC_OK;
}