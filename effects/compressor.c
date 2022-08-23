#include "compressor.h"

CFUNCTION int effect_compressor(slevel_t* samples, size_t length, fheader* header, slevel_t threshold, double ratio, size_t attack, size_t release)
{
    if(samples == NULL || header == NULL) return FUNC_INVALID_ARG;
    if(threshold < 1 || ratio < 1) return FUNC_INVALID_ARG;

    if(ratio == 1) return FUNC_OK;

    uint64_t channels = header->channels;
    uint64_t samplerate = header->samplerate;

    if(length < 1 || channels < 1 || samplerate < 1 || length % channels) return FUNC_INVALID_ARG;
    // Length must be multiple of channels, because
    // number of samples in each channel must be the same.

    double releasestep = (ratio-1)/release;
    double attackstep = (ratio-1)/attack;

    double cur_ratio = 1; bool attacking = false; bool releasing = false;

    for(size_t i = 0; i < length; i += channels)
    {
        slevel_t mabs_level = 0;

        for(uint64_t ci = 0; ci < channels; ci++)
        {
            slevel_t sample = samples[i+ci];

            if(sample == SLEVEL_MIN)
            {
                mabs_level = SLEVEL_MAX;
                break;
            }
            else
            {
                slevel_t abs = imaxabs(samples[i+ci]);

                if(abs > mabs_level) mabs_level = abs;
            }
        }

        if(attacking)
        {
            if(cur_ratio >= ratio)
            {
                attacking = false;
                releasing = true;
            }
            else cur_ratio += attackstep;

            for(uint64_t ci = 0; ci < channels; ci++)
            {
                samples[i+ci] = samples[i+ci]*(slttof64(threshold)*(1+(1/cur_ratio)));
            }
        }
        else if(releasing)
        {
            if(cur_ratio <= 1)
            {
                releasing = false;
                continue;
            }

            cur_ratio -= releasestep;

            for(uint64_t ci = 0; ci < channels; ci++)
            {
                samples[i+ci] = samples[i+ci]*(slttof64(threshold)*(1+(1/cur_ratio)));
            }
        }
        
        if(mabs_level >= threshold)
        {
            attacking = true;
            continue;
        }
    }

    return FUNC_OK;
}