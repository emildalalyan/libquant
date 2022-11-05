#pragma once

// This file contains some definitions from C99/C++11 standards.
// It's needed with the compilers, that don't support C99/C++11
// or support them partially.
// 
// Some of the definitions are compiler-dependent (or machine-dependent, e.g size_t), so
// CHECK ALL DEFINITONS BEFORE COMPILING CODE TO AVOID STRANGE BUGS.
//
// If your compiler is already has some definitions (but not all),
// then just comment corresponding line/block of code.
//
// If your compiler is already has some functions, then you
// have to comment the IMPLEMENTATION, but not the DECLARATION.

/* Floating-point types definition ============ */

typedef double double_t;
typedef float float_t;
// Changing of these types can result to increasing
// or decreasing performance.
// But after changing these types, you have to change
// FLT_EVAL_METHOD definition value.

#undef FLT_EVAL_METHOD
#define FLT_EVAL_METHOD 0

/* ============================================ */

/* Integer types and formats definitions ====== */

// ======================== WARNING!!! ========================
// Your compiler can define standard types with another length.
// If your compiler did, change following strings and type
// definitions to right ones.

#define PRIu8 "hhu"
#define PRIu16 "hu"
#define PRIu32 "u"
#define PRIu64 "llu"

#define PRIi8 "hhi"
#define PRIi16 "hi"
#define PRIi32 "i"
#define PRIi64 "lli"

typedef signed long long int64_t;
typedef signed int int32_t;
typedef signed short int16_t;
typedef signed char int8_t;

typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef unsigned long long size_t;
// If address bus width of target computer is not 64-bit,
// then YOU HAVE TO EDIT THIS DEFINITION, and enter right type
// (i.e type, which length is the same as address bus width).

/* ==================================== */

/* Functions definitions ============== */

double log2(double x);

/* ==================================== */