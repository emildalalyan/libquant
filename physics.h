#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "math.h"
#include "build.h"

/* All values are measured in SI measures.
   Speed: meters per second (m/s)
   Distance: meters (m)
   Time: seconds (s)
   etc. */

/* Sound speed macros ============== */
#define PHYS_SOUND_SPEED_WATER 1403
#define PHYS_SOUND_SPEED_AIR 331
#define PHYS_SOUND_SPEED_HYDROGEN 1284
#define PHYS_SOUND_SPEED_HELIUM 965
#define PHYS_SOUND_SPEED_VACUUM 0
/* ================================= */

/* Calculate sound propagation time.
   For sound speed use macros (for example, PHYS_SOUND_SPEED_AIR is the sound speed in the air).
   Sound speed cannot be less than or equal 0. Distance cannot be less than 0.
   If invalid arguments was provided, then errno will be equate to EINVAL. */
CFUNCTION double physics_sndpropagationtime(double distance, double soundspeed);

/* Constant echo feedback of different materials ==== */
#define PHYS_ECHO_FEEDB_ACOUSTOLITE 0.64
#define PHYS_ECHO_FEEDB_CONCRETE 0.985
#define PHYS_ECHO_FEEDB_GLASS 0.973
/* ================================================== */