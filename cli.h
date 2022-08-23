#pragma once

#include <stdio.h>
#include "samples.h"
#include "io/fheaders.h"

/* Command interpreter macros =========== */

#define MAX_CMD_SYMBOLS 256U
// Number of symbols that will be gathered from console (stdin).

#define CMD_NEXT_MARK "-> "
// Next command marker in console.

/* ====================================== */

/* Start CLI (Command Line Interface).
   Provide "stdin" as input, and "stdout" as output, so that input and output go from/to the console.
   Samples array may be reallocated. Length and header may be changed.
   If echoinput is true, then after getting input, function will send it to output stream. */
CFUNCTION int cli_start(slevel_t** samples, size_t* length, fheader* header, FILE* output, FILE* input, bool echoinput);