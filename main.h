#pragma once

/* Including standard headers */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include "math.h"

#if defined(__cplusplus)
    #define __STDC_FORMAT_MACROS 1
    // In C++, "inttypes.h" doesn't define format macros (e.g. PRIu64) by default, so
    // we should define __STDC_FORMAT_MACROS before including "inttypes.h"
#endif

#include <inttypes.h>

/* Count the time of expression execution, and
   write the result to "counted" variable (of type "double").
   It will be measured in seconds. */
#if defined(_WIN32)
    #define COUNTTIME(expression, counted) \
    { \
        clock_t start = clock(); \
        expression; \
        counted = ((double)clock()-start)/CLOCKS_PER_SEC; \
    }
#else
    #define COUNTTIME(expression, counted) \
    { \
        time_t start = time(NULL); \
        expression; \
        counted = difftime(time(NULL), start); \
    }
    // In Windows, clock() returns process life time. 
    // In *nix systems, clock() returns CPU time (of all threads),
    // that has been spent on process execution,
    // so we have to use time() in *nix systems to calculate
    // time spent on function execution.
#endif

/* ====================================== */
#include "endianness.h"
#include "samples.h"
#include "physics.h"
#include "measures.h"
#include "math.h"
#include "build.h"
#include "cli.h"
/* ====================================== */

// Input/output =========================
#include "io/fheaders.h"
#include "io/wav.h"

// Sound effects ========================
#include "effects/quantize.h"
#include "effects/mixing.h"
#include "effects/volume.h"
#include "effects/basic.h"
#include "effects/echo.h"
#include "effects/phaser.h"
#include "effects/compressor.h"
#include "effects/filters.h"
#include "effects/stretch.h"

// Sound generators (synthesizers) ======
#include "synth/common.h"