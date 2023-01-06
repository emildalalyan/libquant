#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../samples.h"
#include "../io/fheaders.h"

/* Produces multiple echo effect (without equalization).

   Delay measured in samples.
   Feedback is the multiplier of each bounce (relatively to previous bounce).
   Bounces is the number of echo bounces, the more bounces the more iterations will be passed. */
CFUNCTION int effect_echo(slevel_t* samples, size_t length, fheader* header, size_t delay, double feedback, size_t bounces);

/* Produces delay effect (or single echo effect).
   
   Delay is measured in samples.
   Volume of the delay is the multiplier of original samples.
   
   Use it, where you need single echo effect, instead of effect_echo with 1 bounce,
   because in this case, this function is faster than effect_echo. */
CFUNCTION int effect_delay(slevel_t* samples, size_t length, fheader* header, size_t delay, double volume);