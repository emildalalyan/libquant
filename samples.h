#pragma once

#if !defined(__cplusplus)
   #include <stdbool.h>
   // C++ already defines boolean type,
   // so we don't need to include this file.
#endif

#include <stdint.h>
#include <stddef.h>
#include <float.h>
#include "math.h"
#include "openmp.h"
#include "functions.h"
#include "build.h"
#include <errno.h>
#include <stdlib.h>

#if defined(SUPPORTS_C99) || defined(SUPPORTS_CPP11)
   #include <inttypes.h>
#endif

/* This is the type of sample levels. It must be signed integer type. */
typedef int32_t slevel_t;

/* Defining range of level type */

#define SLEVEL_MIN INT32_MIN
// Minimum possible integer, that may
// be stored in slevel_t.

#define SLEVEL_MAX INT32_MAX
// Maximum possible integer, that may
// be stored in slevel_t.

/* ============================ */

#define SLEVEL_BIT_DEPTH 32
// Length of slevel_t integer (in bits).

#define SLEVEL_FORMAT PRIi32
// Format, that may be used in following
// functions: printf, sscanf, etc.

#define INT24_MAX 8388607
#define INT24_MIN -8388608
#define UINT24_MAX 16777215
#define UINT24_MIN 0
// 24-bit integer type is missing in C, but some audio files are 24-bit,
// so these definitions is introduced for code readability.

/* Convert 16-bit signed integer (sample) to slevel_t format. */
CFUNCTION slevel_t i16toslt(int16_t sample);

/* Convert 24-bit signed integer (sample) to slevel_t format.
   If invalid arguments was provided, then errno will be equate to EINVAL. */
CFUNCTION slevel_t i24toslt(int32_t sample);

/* Convert 32-bit signed integer (sample) to slevel_t format. */
CFUNCTION slevel_t i32toslt(int32_t sample);

/* Convert 32-bit floating-point number (sample) to slevel_t format.
   Number must be greater than or equal -1, and less than or equal 1, otherwise
   resulting sample will be SLEVEL_MIN and SLEVEL_MAX accordingly. */
CFUNCTION slevel_t f32toslt(float sample);

/* Convert 64-bit floating-point number (sample) to slevel_t format.
   Number must be greater than or equal -1, and less than or equal 1, otherwise
   resulting sample will be SLEVEL_MIN and SLEVEL_MAX accordingly. */
CFUNCTION slevel_t f64toslt(double sample);

/* Convert slevel_t format sample to 16-bit integer. */
CFUNCTION int16_t slttoi16(slevel_t sample);

/* Convert slevel_t format sample to 24-bit integer. */
CFUNCTION int32_t slttoi24(slevel_t sample);

/* Convert slevel_t format sample to 32-bit integer. */
CFUNCTION int32_t slttoi32(slevel_t sample);

/* Convert slevel_t format sample to 32-bit floating-point number.
   Resulting number will be in range of [-1,1], i.e it won't be
   greater than 1, and less than -1. */
CFUNCTION float slttof32(slevel_t sample);

/* Convert slevel_t format sample to 64-bit floating-point number.
   Resulting number will be in range of [-1,1], i.e it won't be
   greater than 1, and less than -1. */
CFUNCTION double slttof64(slevel_t sample);

/* Free samples array, and set it to NULL to avoid dangling pointer.
   If invalid arguments was provided, then errno will be equate to EINVAL. */
CFUNCTION void samples_free(slevel_t** samples);

/* Find maximum absolute sample level in provided array.
   If invalid arguments was provided, then errno will be equate to EINVAL. */
CFUNCTION slevel_t samples_findmaxabs(slevel_t* samples, size_t length);

/* Compares two samples.
   It returns -1, if first is lesser, it returns 1, if first is greater,
   it returns 0, if first sample equals second one.
   Function does not check whether pointers are NULL or not, so if NULL
   pointers was provided, then you'll get segmentation fault.
   This is because of performance, if this function is using in sorting algorithm,
   then it will be called multiple times, and checks would reduce performance. */
CFUNCTION int samples_compare(const slevel_t* first, const slevel_t* second);

/* Returns absolute value of sample,
   i.e if sample is negative, then this function will change its sign. */
CFUNCTION slevel_t sltabs(slevel_t sample);