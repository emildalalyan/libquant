#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../math.h"
#include "../samples.h"
#include "../io/fheader.h"

/* Single-band peak-sensing compressor effect.
   It produces downward compression (i.e it decreases the volume by the ratio when absolute value of sample becomes higher than threshold).
   Length is the number of samples of all channels.
   Threshold must be positive value.
   Ratio is the signal amplitude reducing factor.
   Attack is the delay between detecting sample, which absolute value greater than or equals threshold and reducing amplitude of the signal,
   it's measured in samples. Release is the delay between attacking and restoring original amplitude of the signal, it's measured in samples. */
CFUNCTION int effect_compressor(slevel_t* samples, size_t length, fheader* header, slevel_t threshold, double ratio, size_t attack, size_t release);