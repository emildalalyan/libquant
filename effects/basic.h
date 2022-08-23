#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../samples.h"
#include "../io/fheaders.h"

/* Sound reverse effect.
   It reverses the samples position (e.g. first sample become last, and so forth). */
CFUNCTION int effect_reverse(slevel_t* samples, fheader* header, size_t length);

/* Signal invert effect.
   It inverts each sample (i.e it changes sample sign,
   e.g. positive sample become negative, and so forth). */
CFUNCTION int effect_invert(slevel_t* samples, size_t length);

/* Mutes the signal.
   Use it to mute the signal instead of effect_amplify with ratio 0.
   It sets the samples to 0. */
CFUNCTION int effect_mute(slevel_t* samples, size_t length);

/* Mixes white noise with the signal.
   Noise volume cannot be less than 0.
   Noise samples will be gathered with "rand" function. */
CFUNCTION int effect_mixnoise(slevel_t* samples, size_t length, double noisevolume);

/* Select samples from one channel in array of interleaving channel samples.
   Length is the number of samples in all channels.
   SAMPLES ARRAY WILL BE FREED, FUNCTION WILL ALLOCATE NEW ONE,
   CHANNELS FIELD IN HEADER AND LENGTH WILL BE CHANGED. 
   "channelnum" is the channel number, that will be selected. */
CFUNCTION int effect_selectchannel(slevel_t** samples, size_t* length, fheader* header, uint64_t channelnum);