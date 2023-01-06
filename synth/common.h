#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <float.h>

#include "../math.h"
#include "../io/fheaders.h"
#include "../samples.h"

/* Generates sinusoidal signal with specified frequency.

   Pointer to samples array must be NULL.
   Frequency cannot be greater than Nyquist frequency. (see common.c)

   Sample rate of output signal and number of channels will be gathered from header. */
CFUNCTION int synth_sinusoid(slevel_t** samples, size_t length, fheader* header, double freq);

/* Generates sawtooth signal with specified frequency.

   Pointer to samples array must be NULL.
   Frequency cannot be greater than Nyquist frequency. (see common.c)

   Sample rate of output signal and number of channels will be gathered from header. */
CFUNCTION int synth_sawtooth(slevel_t** samples, size_t length, fheader* header, double freq);

/* Generates square wave signal with specified frequency.

   Pointer to samples array must be NULL.
   Frequency cannot be greater than Nyquist frequency. (see common.c)

   Sample rate of output signal and number of channels will be gathered from header. */
CFUNCTION int synth_square(slevel_t** samples, size_t length, fheader* header, double freq);

/* Generates silence.

   Pointer to samples array must be NULL. */
CFUNCTION int synth_silence(slevel_t** samples, size_t length);

/* Generates noise.
   
   All pseudo-random values goes from rand().
   
   Pointer to samples array must be NULL. */
CFUNCTION int synth_noise(slevel_t** samples, size_t length);