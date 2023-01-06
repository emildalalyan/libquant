#pragma once

/* All values are measured in SI measures.
   Speed: meters per second (m/s)
   Distance: meters (m)
   Time: seconds (s)
   etc. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "math.h"
#include "build.h"

/* Sound speed macros ============== */

#define PHYS_SOUND_SPEED_WATER 1403
#define PHYS_SOUND_SPEED_AIR 331
#define PHYS_SOUND_SPEED_HYDROGEN 1284
#define PHYS_SOUND_SPEED_HELIUM 965
#define PHYS_SOUND_SPEED_VACUUM 0

/* ================================= */

/* Calculate echo delay for provided distance and sound speed. */
CFUNCTION double physics_getechodelay(double distance, double soundspeed);