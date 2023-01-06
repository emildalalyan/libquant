#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../samples.h"
#include "../io/fheaders.h"

/* Mean filter effect.

   It passes each sample, and replaces it with mean of neighboring samples.
   Number of neighboring samples, whose median will be calculated called "window size"
   
   Last argument is the pointer to mean function.
   You can use, for example, sltmedian as mean function. (see samples.h) */
CFUNCTION int effect_meanfilter(slevel_t* samples, size_t length, fheader* header, size_t windowsize, mean_func_t meanfunction);