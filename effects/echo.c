#include "echo.h"

CFUNCTION int effect_echo(slevel_t* samples, size_t length, fheader* header, size_t delay, double feedback, size_t bounces)
{
    /* Arguments processing ============= */

    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(bounces < 1 || delay < 1 || length < 1) return FUNC_INVALID_ARG;
    if(feedback < 0 || feedback > 1) return FUNC_INVALID_ARG;

    if(feedback == 0) return FUNC_OK;
    // In this case, there will be no echo.

    uint32_t channels = header->channels;
    if(channels < 1 || length % channels) return FUNC_INVALID_ARG;
    // Number of samples in each channel must be the same.

    if(delay > (length/channels)) return FUNC_OK;
    // In this case, main audio signal will end before we
    // would hear a delayed signal, so we won't hear it.
    
    /* ================================== */

    /* Feedback lookup table ============ */

    double* feedb_lut = (double*)malloc(bounces * sizeof(double));
    // We're creating feedbacks lookup table (LUT)
    // Without it, we'd calculate feedbacks in each iteration of the loop.

    if(feedb_lut == NULL) return FUNC_MEMALLOC_FAILED;

    feedb_lut[0] = feedback;
    for(size_t i = 1; i < bounces; i++)
        feedb_lut[i] = feedb_lut[i-1] * feedback;

    /* ================================== */
    
    /* Doing echo effect ================ */

    slevel_t* result = (slevel_t*)malloc(length * sizeof(slevel_t));
    if(result == NULL) return FUNC_MEMALLOC_FAILED;

    #pragma omp parallel for schedule(static)
    for(omp_iter_t i = 0; i < length; i++)
    {
        double_t sample = samples[i];

        for(size_t si = 1; si <= bounces; si++)
        {
            size_t shift = (delay*channels)*si;
            if(i < shift) break;

            sample += (samples[i-shift] * feedb_lut[si-1]);
        }

        SLEVEL_CLIPPING(sample, result[i]);
        // Samples with applied echo can be out of slevel_t range,
        // because echo can be too loud, so we should clip levels.
    }
    
    /* ================================== */
    
    /* Moving result and freeing memory = */

    memcpy(samples, result, (length * sizeof(slevel_t)));
    free(result);
    free(feedb_lut);
    
    /* ================================== */

    return FUNC_OK;
}

CFUNCTION int effect_delay(slevel_t* samples, size_t length, fheader* header, size_t delay, double volume)
{    
    /* Arguments processing ============= */

    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;

    if(volume == 0) return FUNC_OK;
    // In this case, delayed sound will be muted.

    uint32_t channels = header->channels;
    if(channels < 1 || length % channels) return FUNC_INVALID_ARG;
    // Number of samples in each channel must be the same.

    if(delay > (length/channels)) return FUNC_OK;
    // In this case, audio signal will end before we
    // would hear a delayed signal, so we won't hear it.

    size_t shift = delay*channels;
    // Samples in array are interleaved by channels,
    // so we need to multiply delay by channels to get correct shift.
    
    /* ================================== */
    
    /* Doing delay effect =============== */

    slevel_t* result = (slevel_t*)malloc(length * sizeof(slevel_t));
    if(result == NULL) return FUNC_MEMALLOC_FAILED;

    #pragma omp parallel for schedule(static)
    for(omp_iter_t i = 0; i < length; i++)
    {
        if(i < shift)
        {
            result[i] = samples[i];
            continue;
        }

        double mixed = (double)samples[i] + (double)(samples[i-shift] * volume);

        SLEVEL_CLIPPING(mixed, result[i]);
        // Samples with applied echo can be out of slevel_t range,
        // because delayed signal can be too loud, so we should clip levels.
    }
    
    /* ================================== */
    
    /* Moving result and freeing memory = */

    memcpy(samples, result, (length * sizeof(slevel_t)));
    free(result);

    /* ================================== */

    return FUNC_OK;
}