#include "phaser.h"

CFUNCTION int effect_phaser(slevel_t* samples, size_t length, double maxphase, double freq)
{
    if(samples == NULL) return FUNC_INVALID_ARG;
    if(length < 1 || maxphase < 0 || maxphase > 360) return FUNC_INVALID_ARG;
    if(freq <= 0) return FUNC_INVALID_ARG;
    
    if(maxphase == 0) return FUNC_OK;
    // If maxphase is 0, then there will be no effect.

    

    return FUNC_OK;
}