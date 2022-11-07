#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../io/fheaders.h"
#include "../samples.h"

/* Mixdown samples to specified number of channels.
   Number of channels in header will be changed to provided value
   Samples array WILL BE FREED, and function will
   allocate new one (its size will be changed). */
CFUNCTION int effect_chamixdown(slevel_t** samples, fheader* header, size_t* length, uint64_t newchannels);

/* Channel difference effect.
   It calculates difference between sample and arithmetic mean of samples from all channels. */
CFUNCTION int effect_chadiff(slevel_t* samples, size_t length, fheader* header);