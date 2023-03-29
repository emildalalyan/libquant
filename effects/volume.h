#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../samples.h"
#include "../io/fheader.h"

/* Auto-amplify effect, or normalization effect.

   Amplifies signal to maximum amplitude, without clipping and overflows. */
CFUNCTION int effect_ampmax(slevel_t* samples, size_t length);

/* Sound amplify effect.

   It multiplies samples by provided multiplier,
   if they don't fit into slevel_t range, they will be clipped. */
CFUNCTION int effect_amplify(slevel_t* samples, size_t length, double multiplier);

/* Fade in/out effect.

   Volume argument is coefficient, in range [0, 1].
   If reverse is true, then effect will become fade out.
   Last argument is the interpolation method, that will be used by function. */
CFUNCTION int effect_fade(slevel_t* samples, size_t length, fheader* header, double initvolume, size_t duration, bool reverse, interp_method method);

/* Sound clipping effect.

   You can read about it there: https://en.wikipedia.org/wiki/Clipping_(signal_processing)
   Maximum sample level must be positive or 0, and minimum must be negative or 0. */
CFUNCTION int effect_clipping(slevel_t* samples, size_t length, slevel_t max, slevel_t min);

/* Flowing sound effect.

   Volume of even channels will be changing by sin^2(x) function,
   and odd channels will be doing it by cos^2(x) function.
   
   This effect does not work with mono audio. */
CFUNCTION int effect_flow(slevel_t* samples, size_t length, fheader* header, double freq);