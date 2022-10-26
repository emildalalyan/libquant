#include "samples.h"

CFUNCTION slevel_t i16toslt(int16_t sample)
{
    if(sample == 0) return (slevel_t)0;

    #if SLEVEL_BIT_DEPTH >= 16
        return ((slevel_t)sample << (SLEVEL_BIT_DEPTH-16));
    #else
        return (slevel_t)((sample >> (16-SLEVEL_BIT_DEPTH)) & ~((slevel_t)0));
        // ~((slevel_t)0) is number where all bits are set to 1
        // even on ones' complement machines.
    #endif
}

CFUNCTION slevel_t i24toslt(int32_t sample)
{
    if(sample == 0) return (slevel_t)0;

    if(sample & UINT8_MAX)
    // Checking, that number is actually 24-bit, and
    // doesn't use first byte of the 4-byte integer
    {
        errno = EINVAL;
        return (slevel_t)0;
    }
    else errno = 0;
    
    #if SLEVEL_BIT_DEPTH >= 32
        return ((slevel_t)sample << (SLEVEL_BIT_DEPTH-32));
    #else
        return (slevel_t)((sample >> (32-SLEVEL_BIT_DEPTH)) & ~((slevel_t)0));
        // (SLEVEL_MIN | SLEVEL_MAX) is number where all bits are set to 1
        // even on ones' complement machines.
    #endif
}

CFUNCTION slevel_t i32toslt(int32_t sample)
{
    if(sample == 0) return (slevel_t)0;

    #if SLEVEL_BIT_DEPTH >= 32
        return ((slevel_t)sample << (SLEVEL_BIT_DEPTH-32));
    #else
        return (slevel_t)((sample >> (32-SLEVEL_BIT_DEPTH)) & ~((slevel_t)0));
        // (SLEVEL_MIN | SLEVEL_MAX) is number where all bits are set to 1
        // even on ones' complement machines.
    #endif
}

CFUNCTION slevel_t f32toslt(float sample)
{
    if(sample == 0.0F) return (slevel_t)0;
    
    float result = sample * SLEVEL_MAX;

    if(result > SLEVEL_MAX) return SLEVEL_MAX;
    else if(result < SLEVEL_MIN) return SLEVEL_MIN;
    // Floating-point numbers is not accurate, so after
    // multiplying, it can be out of slevel_t range
    
    return (slevel_t)result;
}

CFUNCTION slevel_t f64toslt(double sample)
{
    if(sample == 0.0F) return (slevel_t)0;
    
    double result = sample * SLEVEL_MAX;

    if(result > SLEVEL_MAX) return SLEVEL_MAX;
    else if(result < SLEVEL_MIN) return SLEVEL_MIN;
    // Floating-point numbers is not accurate, so after
    // multiplying, it can be out of slevel_t range
    
    return (slevel_t)result;
}

CFUNCTION int16_t slttoi16(slevel_t sample)
{
    if(sample == 0) return (int16_t)0;

    #if SLEVEL_BIT_DEPTH >= 16
        return (int16_t)((sample >> (SLEVEL_BIT_DEPTH-16)) & UINT16_MAX);
    #else
        return ((int16_t)sample << (16-SLEVEL_BIT_DEPTH));
    #endif
}

CFUNCTION int32_t slttoi24(slevel_t sample)
{
    if(sample == 0) return (int32_t)0;

    #if SLEVEL_BIT_DEPTH >= 32
        return (int32_t)((sample >> (SLEVEL_BIT_DEPTH-32)) & (~((int32_t)UINT8_MAX)));
    #else
        return (((int32_t)sample << (32-SLEVEL_BIT_DEPTH)) & (~((int32_t)UINT8_MAX)));
    #endif
}

CFUNCTION int32_t slttoi32(slevel_t sample)
{
    if(sample == 0) return (int32_t)0;

    #if SLEVEL_BIT_DEPTH >= 32
        return (int32_t)((sample >> (SLEVEL_BIT_DEPTH-32)) & UINT32_MAX);
    #else
        return ((int32_t)sample << (32-SLEVEL_BIT_DEPTH));
    #endif
}

CFUNCTION float slttof32(slevel_t sample)
{
    if(sample == 0) return 0.0f;

    float result = ((float)sample/SLEVEL_MAX);

    if(result > 1) return 1;
    else if(result < -1) return -1;
    else return result;
    // Floating-point numbers is not accurate,
    // so after dividing, it can be out of range.
}

CFUNCTION double slttof64(slevel_t sample)
{
    if(sample == 0) return 0.0;

    double result = ((double)sample/SLEVEL_MAX);

    if(result > 1) return 1;
    else if(result < -1) return -1;
    else return result;
    // Floating-point numbers is not accurate,
    // so after dividing, it can be out of range.
}

CFUNCTION void samples_free(slevel_t** samples)
{
    if(samples == NULL || (*samples) == NULL)
    {
        errno = EINVAL;
        return;
    }
    else errno = 0;

    free((*samples));
    (*samples) = NULL;
}

CFUNCTION slevel_t samples_findmaxabs(slevel_t* samples, size_t length)
{
    if(samples == NULL || length < 1)
    {
        errno = EINVAL;
        return 0;
    }
    else errno = 0;

    slevel_t maxabs = 0;

    for(omp_iter_t i = 0; i < length; i++)
    {
        if(samples[i] == SLEVEL_MIN || samples[i] == SLEVEL_MAX)
        // If sample equals SLEVEL_MAX or SLEVEL_MIN, then we don't need
        // to search further, because slevel_t cannot be greater
        // than SLEVEL_MAX or less than SLEVEL_MIN.
        {
            maxabs = SLEVEL_MAX;
            // SLEVEL_MAX is the maximum possible absolute
            // number, that can be found in array of slevel_t.

            break;
        }
        else
        {
            slevel_t abs_sample = sltabs(samples[i]);
            
            if(abs_sample > maxabs) maxabs = abs_sample;
        }
    }
    
    return maxabs;
}

CFUNCTION int samples_compare(const slevel_t* first, const slevel_t* second)
{
    slevel_t val_first = *first;
    slevel_t val_second = *second;

    if(val_first == val_second) return 0;
    else if(val_first > val_second) return 1;
    else return -1;
}

CFUNCTION slevel_t sltabs(slevel_t sample)
{
    if(sample == SLEVEL_MIN) return SLEVEL_MAX;
    // On two's complement machines, -SLEVEL_MIN causes
    // integer overflow, so we have to bypass this.

    if(sample < 0) return -sample;
    else return sample;
}