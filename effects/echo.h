#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../samples.h"
#include "../io/fheaders.h"

/* Produces sound echo effect (without equalization).
   Simulates multiple echo effect. Delay measured in samples.
   Feedback cannot be less than 0, or greater than 1.
   Bounces is the number of echo bounces, the more bounces the more iterations will be passed.
   Recommended number of bounces is ceil(log_feedback(0.01)). */
CFUNCTION int effect_echo(slevel_t* samples, size_t length, fheader* header, size_t delay, double feedback, size_t bounces);

/* Produces delay effect (or single echo effect).
   Delay is measured in samples, it cannot be greater than number of samples of one channel.
   "Volume" is the volume of delayed sound.
   Use it, where you need single echo effect, instead of effect_echo with 1 bounce, because
   in this case, this function is faster than effect_echo. */
CFUNCTION int effect_delay(slevel_t* samples, size_t length, fheader* header, size_t delay, double volume);