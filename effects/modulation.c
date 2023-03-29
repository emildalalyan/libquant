#include "echo.h"

CFUNCTION int effect_chorus(slevel_t* samples, size_t length, fheader* header, size_t mindelay, size_t maxdelay, double modvolume, double freq)
{
    /* Arguments processing ============= */

    if(samples == NULL || header == NULL
    || maxdelay < 1 || mindelay < 1 || length < 1
    || modvolume < 0 || modvolume > 1 || freq <= 0)
        return FUNC_INVALID_ARG;

    if(modvolume == 0) return FUNC_OK;
    // In this case, effect will not affect signal.

    uint32_t channels = header->channels;
    uint32_t samplerate = header->samplerate;

    if(channels < 1 || length % channels || samplerate < 1) return FUNC_INVALID_ARG;
    // Number of samples in each channel must be the same.

    if((maxdelay+mindelay) > (length/channels)) return FUNC_OK;
    // In this case, main audio signal will end before we
    // would hear a delayed signal, so we won't hear it.
    
    /* ================================== */
    
    /* Sine lookup table ================ */

    double step = (MATH_PI*freq)/(samplerate);
    size_t lut_length = (samplerate/freq);

    double* sine_lut = (double*)malloc(lut_length * sizeof(double));
    if(sine_lut == NULL) return FUNC_MEMALLOC_FAILED;

    for(size_t i = 0; i < lut_length; i++)
    {
        double sine = sin(i*step);

        sine_lut[i] = sine*sine;
    }

    /* ================================== */

    /* Doing effect ===================== */

    slevel_t* result = (slevel_t*)malloc(length * sizeof(slevel_t));
    if(result == NULL) return FUNC_MEMALLOC_FAILED;

    #pragma omp parallel for schedule(static)
    for(omp_iter_t i = 0; i < length; i++)
    {
        double mod = sine_lut[(i/channels) % lut_length];

        size_t shift = channels*((maxdelay*mod)+(mindelay));
        shift -= shift % channels;

        if(i < shift) continue;

        double_t sample = samples[i] + (samples[i-shift] * modvolume);

        SLEVEL_CLIPPING(sample, result[i]);
        // Samples with applied echo can be out of slevel_t range,
        // because echo can be too loud, so we should clip levels.
    }
    
    /* ================================== */
    
    /* Moving result and freeing memory = */

    memcpy(samples, result, (length * sizeof(slevel_t)));
    free(result);
    free(sine_lut);

    /* ================================== */

    return FUNC_OK;
}