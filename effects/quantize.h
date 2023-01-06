#pragma once

#include <stdint.h>
#include <stddef.h>
#include "../samples.h"
#include "basic.h"
#include "../io/fheaders.h"
#include "../math.h"

/* Produces quantization effect.

   It leaves only specified number of Most Significant Bit(s)
   and aligns samples for slevel_t format.
   
   If dithering is true, then signal will be mixed with noise
   using effect_mixnoise function, to reduce non-linear distortion. */
CFUNCTION int effect_quantize(slevel_t* samples, size_t length, size_t depth, bool dithering);