#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../samples.h"
#include "../io/fheader.h"

/* Sound reverse effect.

   It reverses the samples position (e.g. first sample becomes last, and so forth). */
CFUNCTION int effect_reverse(slevel_t* samples, fheader* header, size_t length);

/* Signal invert effect.

   It inverts each sample (i.e it changes their sign). */
CFUNCTION int effect_invert(slevel_t* samples, size_t length);

/* Mutes the signal.

   It sets the samples to 0.
   Use it to mute the signal instead of effect_amplify with multiplier 0. */
CFUNCTION int effect_mute(slevel_t* samples, size_t length);

/* Mixes noise with the signal.

   Noise samples will be gathered with "rand" function. */
CFUNCTION int effect_mixnoise(slevel_t* samples, size_t length, double noisevolume);

/* Select samples from one channel.

   It moves samples from specified channel into resulting array,
   and then frees original array, changes its pointer and length. */
CFUNCTION int effect_selectchannel(slevel_t** samples, size_t* length, fheader* header, uint32_t channelnum);