#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../samples.h"
#include "../io/fheaders.h"

/* Median filter effect.
   It passes each sample, and replaces it with median of neighboring samples.
   Number of neighboring samples, whose median will be calculated called "window size"
   Length is the number of samples in all channels.
   Window size must be greater than 0 and less than number of samples in one channel. */
CFUNCTION int effect_medianfilter(slevel_t* samples, size_t length, fheader* header, size_t windowsize);