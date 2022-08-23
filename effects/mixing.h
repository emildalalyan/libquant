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
   It calculates arithmetic mean between the samples from each channel,
   and then subtracts it from samples.
   IT WILL FREE THE SAMPLES ARRAY AND ALLOCATE NEW ONE (WITH ONLY ONE CHANNEL). */
CFUNCTION int effect_chadiff(slevel_t** samples, size_t* length, fheader* header);