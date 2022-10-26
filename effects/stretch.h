#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../io/fheaders.h"
#include "../samples.h"

/* Produces audio time-stretching (only speeds up), not changing sound pitch.
   It takes only "staying" number of samples from each chunk and does cross-fading between staying and unclaimed
   samples, to avoid non-linear distortions. Unclaimed samples is samples, that was cut off.
   Number of samples in chunk, and number of remaining samples cannot be less than 1.
   Number of samples in chunk must be at most greater than number of staying samples in 2 times.
   This function will FREE samples array, allocate new one and will change the length. */
CFUNCTION int effect_speedup(slevel_t** samples, size_t* length, fheader* header, size_t chunksize, size_t staying, interp_method fademethod);