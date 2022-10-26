#include "quantize.h"

CFUNCTION int effect_quantize(slevel_t* samples, size_t length, size_t depth, bool dithering)
{
    if(depth < 1 || depth >= SLEVEL_BIT_DEPTH) return FUNC_INVALID_ARG;
    if(samples == NULL) return FUNC_INVALID_ARG;
    if(length < 1) return FUNC_INVALID_ARG;

    slevel_t combs = (slevel_t)pow(2.0, (double)depth);
    // Number of combinations of I bits: N = 2^I (Hartley Formula)
    // where N - number of combinations, I - number of bits.
    // Explicit conversion of pow() arguments is added for
    // compatibility with C++ compilers.

    if(dithering && (effect_mixnoise(samples, length, (1.0/combs)) != FUNC_OK))
    // Dithering is just mixing noise before quantization.
    // Conditional "and" operator executes second
    // operand (expression) ONLY if first operand is true.
        return FUNC_INTERNAL_ERROR;

    slevel_t bitmask = ((combs-1) << (SLEVEL_BIT_DEPTH-depth));
    // We use bit-masking to select only "depth" of bits.

    slevel_t alignbit = ((slevel_t)1 << (SLEVEL_BIT_DEPTH-depth-1));
    // It's alignment bit, it's necessary, because after applying bitmask
    // both positive and negative numbers become lesser
    // (on one's complement and two's complement machines), so positive-half of wave
    // become less than negative one on half of quantization step.
    // so we have to shift signal on half of quantization step,
    // i.e set previous bit (before last changing bit) to 1.

    #pragma omp parallel for schedule(static)
    for(omp_iter_t i = 0; i < length; i++)
    {
        slevel_t sample = samples[i];

        sample &= bitmask;
        sample |= alignbit;
        // This works only on one's complement and two's complement machines.

        samples[i] = sample;
    }

    return FUNC_OK;
}