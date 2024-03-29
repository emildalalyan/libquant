#pragma once

/* Including standard headers =========== */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

/* ====================================== */

/* Definitions ========================== */

/* Count the time of expression execution, and
   write the result to "counter" variable (of type "double").
   It will be measured in seconds. */
#if defined(_WIN32)
    #define COUNTTIME(expression, counter) \
    { \
        clock_t start = clock(); \
        expression; \
        counter = ((double)clock()-start)/CLOCKS_PER_SEC; \
    }
    // In Windows, clock() returns process life time.
#else
    #define COUNTTIME(expression, counter) \
    { \
        time_t start = time(NULL); \
        expression; \
        counter = difftime(time(NULL), start); \
    }
    // In other systems, clock() returns CPU time (of all threads),
    // that has been spent on process execution,
    // so we have to use time() to calculate
    // time spent on function execution.
#endif

/* ====================================== */

/* Common headers ======================= */

#include "endianness.h"
#include "samples.h"
#include "physics.h"
#include "measures.h"
#include "math.h"
#include "build.h"

/* ====================================== */

/* Input/output ========================= */

#include "io/fheader.h"
#include "io/wav.h"

/* ====================================== */

/* Sound effects ======================== */

#include "effects/quantize.h"
#include "effects/mixing.h"
#include "effects/volume.h"
#include "effects/basic.h"
#include "effects/echo.h"
#include "effects/modulation.h"
#include "effects/compressor.h"
#include "effects/filters.h"
#include "effects/stretch.h"

/* ====================================== */

/* Sound generators (synthesizers) ====== */

#include "synth/common.h"

/* ====================================== */