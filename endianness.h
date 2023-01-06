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

/* Swap 2 bytes by specified address. */
CFUNCTION void pswap_16b(void* num);

/* Swap 4 bytes by specified address. */
CFUNCTION void pswap_32b(void* num);

/* Swap 8 bytes by specified address. */
CFUNCTION void pswap_64b(void* num);

#if defined(CHAR_BIT) && CHAR_BIT != 8
    #error This machine has non-8-bit byte, so this machine is not supported.
#endif

/* Endianness detection macros ======= */

#define ORDER_LE 1234
#define ORDER_BE 4321
#define ORDER_PDP 3412
// Definition of byte order macros.

/* Use this to change default endianness,
   in the case, when endianness cannot be determined. */
#define DEFAULT_ENDIANNESS ORDER_LE

#if defined(__BYTE_ORDER__)
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        #define ENDIANNESS ORDER_LE
    #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        #define ENDIANNESS ORDER_BE
    #elif __BYTE_ORDER__ == __ORDER_PDP_ENDIAN__
        #define ENDIANNESS ORDER_PDP
        #error PDP-endian machines are not supported by this release.
    #else
        #error Machine endianness is unsupported.
    #endif
#else
    #define ENDIANNESS DEFAULT_ENDIANNESS

    COMPILER_WARNING("Machine endianness is not defined, it has been set to default")
#endif

/* =================================== */

/* Get string name of used byte-order. */
CFUNCTION const char* endianness_getname();