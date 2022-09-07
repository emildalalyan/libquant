#include "common.h"

// ::: Nyquist Theorem | Kotelnikov Theorem :::::::::::::::::::::::::::::::::::::::::::
// : Frequency cannot be greater than Nyquist frequency for current sample rate.      :
// : Nyquist frequency equals one half of sample rate.                                :
// : If frequency would be greater than Nyquist frequency, then sample rate           :
// : would be insufficient for it, aliasing effect would appear, because for correct  :
// : representation of signal, you need at least two samples for each oscillation.    :
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CFUNCTION int synth_sinusoid(slevel_t** samples, size_t length, fheader* header, double freq)
{
    if((*samples) != NULL || samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;

    uint64_t channels = header->channels;
    uint64_t samplerate = header->samplerate;

    if(channels < 1 || samplerate < 1) return FUNC_INVALID_ARG;

    if(length % channels) return FUNC_INVALID_ARG;
    // Length must be multiple of channels, because
    // number of samples in each channel must be the same.

    if(freq <= 0 || freq > (samplerate/2.0)) return FUNC_INVALID_ARG;
    // For explanation, see line 3.

    slevel_t* result = (slevel_t*)malloc(length * sizeof(slevel_t));
    if(result == NULL) return FUNC_MEMALLOC_FAILED;
    // If allocation was failed, malloc will return NULL pointer

    double step = ((2*MATH_PI)*freq)/(samplerate*channels);
    // Period of sin(x) function is 2*pi

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < length; i += channels)
    {
        slevel_t sample = f64toslt(sin(i*step));

        for(uint64_t ci = 0; ci < channels; ci++)
        {
            result[i+ci] = sample;
        }
    }

    (*samples) = result;

    return FUNC_OK;
}

CFUNCTION int synth_sawtooth(slevel_t** samples, size_t length, fheader* header, double freq)
{
    if((*samples) != NULL || samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;

    uint64_t channels = header->channels;
    uint64_t samplerate = header->samplerate;

    if(channels < 1 || samplerate < 1) return FUNC_INVALID_ARG;

    if(length % channels) return FUNC_INVALID_ARG;
    // Length must be multiple of channels, because
    // number of samples in each channel must be the same.

    if(freq <= 0 || freq > (samplerate/2.0)) return FUNC_INVALID_ARG;
    // For explanation, see line 3.

    slevel_t* result = (slevel_t*)malloc(length * sizeof(slevel_t));
    
    if(result == NULL) return FUNC_MEMALLOC_FAILED;
    // If allocation was failed, malloc will return NULL pointer

    double period = ((double)(samplerate)/freq);

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < length; i += channels)
    {
        double sample = ((fmod(i/channels, period)) - (period/2)) * (SLEVEL_MAX/(period/2));

        for(uint64_t ci = 0; ci < channels; ci++)
        {
            if(sample > SLEVEL_MAX) result[i+ci] = SLEVEL_MAX;
            else if(sample < SLEVEL_MIN) result[i+ci] = SLEVEL_MIN;
            else result[i+ci] = (slevel_t)sample;
            // Floating-point numbers are inaccurate so we have to
            // check whether it's out of slevel_t range or not.
        }
    }

    (*samples) = result;

    return FUNC_OK;
}

CFUNCTION int synth_square(slevel_t** samples, size_t length, fheader* header, double freq)
{
    if((*samples) != NULL || samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;

    uint64_t channels = header->channels;
    uint64_t samplerate = header->samplerate;

    if(channels < 1 || samplerate < 1) return FUNC_INVALID_ARG;

    if(length % channels) return FUNC_INVALID_ARG;
    // Length must be multiple of channels, because
    // number of samples in each channel must be the same.

    if(freq <= 0 || freq > (samplerate/2.0)) return FUNC_INVALID_ARG;
    // For explanation, see line 3.

    slevel_t* result = (slevel_t*)malloc(length * sizeof(slevel_t));
    
    if(result == NULL) return FUNC_MEMALLOC_FAILED;
    // If allocation was failed, malloc will return NULL pointer

    double period = (samplerate/freq);
    // Storages square wave signal period.

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < length; i += channels)
    {
        slevel_t sample = (fmod((i/channels), period) >= (period/2)) ? SLEVEL_MAX : SLEVEL_MIN;

        for(uint64_t ci = 0; ci < channels; ci++)
        {
            result[i+ci] = (slevel_t)sample;
        }
    }

    (*samples) = result;

    return FUNC_OK;
}

CFUNCTION int synth_silence(slevel_t** samples, size_t length)
{
    if(samples == NULL || (*samples) != NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;

    (*samples) = (slevel_t*)calloc(length, sizeof(slevel_t));
    // calloc allocates memory with bytes initialized to 0.

    return FUNC_OK;
}

CFUNCTION int synth_noise(slevel_t** samples, size_t length)
{
    if(samples == NULL || (*samples) != NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;

    slevel_t* result = (slevel_t*)malloc(length * sizeof(slevel_t));
    
    if(result == NULL) return FUNC_MEMALLOC_FAILED;
    // If allocation was failed, malloc will return NULL pointer

    #pragma omp parallel for schedule(static)
    for(size_t i = 0; i < length; i++)
    {
        double_t sample = rand();
        // 'double_t' is most efficient floating-point
        // type, at least as wide as double. 
        // We use floating-point type for random number, because
        // rand returns "int". slevel_t can be shorter than "int", and
        // vice versa, so it can lead to integer overflows.

        sample -= (RAND_MAX/2.0);
        sample *= (SLEVEL_MAX/(RAND_MAX/2.0));
        // We use (RAND_MAX/2.0), and not (RAND_MAX/2), for implicit
        // conversion to double-precision floating-point
        // number, because results can be fractional.

        if(sample > SLEVEL_MAX) result[i] = SLEVEL_MAX;
        else if(sample < SLEVEL_MIN) result[i] = SLEVEL_MIN;
        else result[i] = sample;
        // Floating point numbers are inaccurate, so we
        // have to check whether it's out of slevel_t range or not.
    }

    (*samples) = result;

    return FUNC_OK;
}