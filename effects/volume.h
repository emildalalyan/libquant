#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../samples.h"
#include "../io/fheaders.h"

/* Auto-amplify effect.
   Amplifies signal to maximum amplitude, without clipping and overflows. */
CFUNCTION int effect_ampmax(slevel_t* samples, size_t length);

/* Sound amplify effect.
   If multiplied samples don't fit into slevel_t range, they'll be clipped
   (i.e. if sample greater than max, it'll be set to max, and so on). */
CFUNCTION int effect_amplify(slevel_t* samples, size_t length, double ratio);

/* Fade in/out effect.
   "initvolume" is the initial volume coefficient of the signal.
   Initial volume coefficient cannot be less than 0, and greater than 1.
   If reverse is true, then effect become fade out.
   Last arguments is the interpolation method, i.e, in this case, intermediate volume finding method. */
CFUNCTION int effect_fade(slevel_t* samples, size_t length, fheader* header, double initvolume, size_t duration, bool reverse, interp_method method);

/* Sound distortion effect (produces clipping effect).
   Max sample level must be positive number or 0.
   Min sample level must be negative number or 0. */
CFUNCTION int effect_distort(slevel_t* samples, size_t length, slevel_t max, slevel_t min);