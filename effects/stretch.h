#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../io/fheader.h"
#include "../samples.h"

/* Produces audio speed-up, without changing sound pitch.

   This function splits sound into chunks, and moves only neccessary samples
   into resulting array. Number of neccessary samples called "remaining".
   
   To avoid level jumps, function does cross-fading of leaved samples
   with moved ones, using provided interpolation method.

   This function will FREE samples array, allocate new one and will change the length. */
CFUNCTION int effect_speedup(slevel_t** samples, size_t* length, fheader* header, size_t chunksize, size_t remaining, interp_method fademethod);