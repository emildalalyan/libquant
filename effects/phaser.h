#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../samples.h"
#include "../io/fheaders.h"

/* Sound phaser effect.
   Max phase is the maximum phase of the delayed signal, that will be
   mixed with the original one. Frequency is phase change frequency.
   Max phase cannot be 0, or less than 0. Frequency cannot be less than 0. */
CFUNCTION int effect_phaser(slevel_t* samples, size_t length, double maxphase, double freq);