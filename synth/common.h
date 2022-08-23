#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <float.h>
#include "../math.h"

#include "../io/fheaders.h"
#include "../samples.h"

/* Generates sinusoidal signal with specified frequency.
   Samples array must be NULL. Frequency cannot be less than or equal 0, or greater than Nyquist frequency.
   Length is the number of samples in all channels. 
   Sample rate of output signal and number of channels will be gathered from header. */
CFUNCTION int synth_sinusoid(slevel_t** samples, size_t length, fheader* header, double freq);

/* Generates sawtooth signal with specified frequency.
   Samples array must be NULL. Frequency cannot be less than or equal 0, or greater than Nyquist frequency.
   Length is the number of samples in all channels. 
   Sample rate of output signal and number of channels will be gathered from header. */
CFUNCTION int synth_sawtooth(slevel_t** samples, size_t length, fheader* header, double freq);

/* Generates square wave signal with specified frequency.
   Samples array must be NULL. Frequency cannot be less than or equal 0, or greater than Nyquist frequency.
   Length is the number of samples in all channels. 
   Sample rate of output signal and number of channels will be gathered from header. */
CFUNCTION int synth_square(slevel_t** samples, size_t length, fheader* header, double freq);

/* Generates silence.
   Samples array must be NULL.
   Length is the number of samples in all channels. */
CFUNCTION int synth_silence(slevel_t** samples, size_t length);

/* Generates noise.
   Samples array must be NULL. All pseudo-random values goes from rand().
   Length is the number of samples in all channels. */
CFUNCTION int synth_noise(slevel_t** samples, size_t length);