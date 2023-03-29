#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../samples.h"
#include "../io/fheader.h"

/* Chorus effect.
   It is experimental by now, so code may be optimized or heavily changed later.
   
   Low frequency oscillator can create only sine waveform. 
   Also, this effect has only 1 voice mode. */
CFUNCTION int effect_chorus(slevel_t* samples, size_t length, fheader* header, size_t mindelay, size_t maxdelay, double modvolume, double freq);