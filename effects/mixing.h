#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../io/fheaders.h"
#include "../samples.h"

/* Mixdown audio to specified number of channels.

   Number of channels in header will be changed to provided value.
   Original samples array will be freed, and function will allocate new one. */
CFUNCTION int effect_chamixdown(slevel_t** samples, fheader* header, size_t* length, uint64_t newchannels);

/* Extract channel difference.

   It calculates difference between sample and arithmetic mean of samples from all channels. */
CFUNCTION int effect_extractchadiff(slevel_t* samples, size_t length, fheader* header);

/* Amplify channel difference.

   It amplifies difference between sample and arithmetic mean
   of samples from all channels, i.e multiplies it by specified multiplier. */
CFUNCTION int effect_amplifychadiff(slevel_t* samples, size_t length, fheader* header, double multiplier);