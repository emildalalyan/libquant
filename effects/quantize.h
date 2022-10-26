#pragma once

#include <stdint.h>
#include <stddef.h>
#include "../samples.h"
#include "basic.h"
#include "../io/fheaders.h"
#include "../math.h"

/* Quantization effect.
   It quantizes the samples, and then converts it to slevel_t.
   If dithering is true, then signal will be mixed with noise
   using effect_mixnoise function, to reduce non-linear distortion. */
CFUNCTION int effect_quantize(slevel_t* samples, size_t length, size_t depth, bool dithering);