#pragma once

#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include "build.h"

/* Swap 2 bytes in specified number. */
CFUNCTION uint16_t swap_16b(uint16_t num);

/* Swap 4 bytes in specified number */
CFUNCTION uint32_t swap_32b(uint32_t num);

/* Swap 8 bytes in specified number */
CFUNCTION uint64_t swap_64b(uint64_t num);

/* Swap 4 bytes in floating-point number. */
CFUNCTION float fswap_32b(float num);

/* Swap 8 bytes in floating-point number. */
CFUNCTION double fswap_64b(double num);

/* Swap 2 bytes in specified signed number. */
CFUNCTION int16_t iswap_16b(int16_t num);

/* Swap 4 bytes in specified signed number */
CFUNCTION int32_t iswap_32b(int32_t num);

/* Swap 8 bytes in specified signed number */
CFUNCTION int64_t iswap_64b(int64_t num);

#if !defined(__ORDER_LITTLE_ENDIAN__)
    #define __ORDER_LITTLE_ENDIAN__ 1234
#endif

#if !defined(__ORDER_BIG_ENDIAN__)
    #define __ORDER_BIG_ENDIAN__ 4321
#endif

#if !defined(__ORDER_PDP_ENDIAN__)
    #define __ORDER_PDP_ENDIAN__ 3412
#endif
// These macros are not defined in MSVC.

#if defined(CHAR_BIT) && CHAR_BIT != 8
    #error This machine has non-8-bit byte, so this machine is not supported.
#endif

/* With these macros, you can get machine endianness. */
#if defined(__BYTE_ORDER__)
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        #define ENDIANNESS __ORDER_LITTLE_ENDIAN__
    #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        #define ENDIANNESS __ORDER_BIG_ENDIAN__
    #elif __BYTE_ORDER__ == __ORDER_PDP_ENDIAN__
        #define ENDIANNESS __ORDER_PDP_ENDIAN__
        #error PDP-endian machines are not supported by this release.
    #else
        #error Machine endianness is unsupported.
    #endif
#else
    COMPILER_WARNING("Machine endianness is not defined, by default it is little-endian")

    #define ENDIANNESS __ORDER_LITTLE_ENDIAN__
#endif

/* Get string name of used byte-order (endianness). */
CFUNCTION const char* endianness_getname();