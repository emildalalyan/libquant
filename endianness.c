#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "endianness.h"

CFUNCTION uint16_t swap_16b(uint16_t num)
{
    #if (defined(ARCH_X86) || defined(ARCH_X86_64)) && defined(SUPPORTS_GCC)
        uint32_t result = 0;
        __asm__("rol $8, %%ax" : "=a"(result) : "a"(num));

        return result;
    #elif defined(ARCH_X86) && defined(SUPPORTS_MSVC)
        __asm
        {
            rol num, 8
        }

        return num;
    #else
        return  ((num & 0x00FF)<<8) |
                ((num & 0xFF00)>>8) ;
    #endif
}

CFUNCTION uint32_t swap_32b(uint32_t num)
{
    #if (defined(ARCH_X86) || defined(ARCH_X86_64)) && defined(SUPPORTS_GCC)
        uint32_t result = 0;
        __asm__("bswap %%eax" : "=a"(result) : "a"(num));

        return result;
    #elif defined(ARCH_X86) && defined(SUPPORTS_MSVC)
        uint32_t result = 0;
        __asm
        {
            mov eax, num
            bswap eax
            mov result, eax
        }

        return result;
    #else
        return  ((num & 0xFF000000)>>24) |
                ((num & 0x00FF0000)>>8)  |
                ((num & 0x0000FF00)<<8)  |
                ((num & 0x000000FF)<<24) ;
    #endif
}

CFUNCTION uint64_t swap_64b(uint64_t num)
{
    #if defined(ARCH_X86_64) && defined(SUPPORTS_GCC)
        uint64_t result = 0;
        __asm__("bswap %%rax" : "=a"(result) : "a"(num));

        return result;
    #elif defined(ARCH_X86) && defined(SUPPORTS_GCC)
        uint32_t res_high = 0, res_low = 0;
        res_high = (num >> 32) & UINT32_MAX;
        res_low = num & UINT32_MAX;
        // We divide 64-bit number into low and high 32-bit words.
        
        __asm__("bswap %%eax\n"
                "bswap %%edx" : "=a"(res_high), "=d"(res_low) : "a"(res_high), "d"(res_low));

        return (((uint64_t)res_low << 32) | (uint64_t)res_high);
        // And then we swap them.
    #elif defined(ARCH_X86) && defined(SUPPORTS_MSVC)
        uint32_t res_high = 0, res_low = 0;

        res_high = (num >> 32) & UINT32_MAX;
        res_low = num & UINT32_MAX;
        // We divide 64-bit number into low and high 32-bit words.

        __asm
        {
            mov eax, res_high
            mov edx, res_low
            bswap eax
            bswap edx
            mov res_high, eax
            mov res_low, edx
        }

        return (((uint64_t)res_low << 32) | (uint64_t)res_high);
        // And then we swap them.
    #else
        return  ((num & 0xFF00000000000000)>>56) |
                ((num & 0x00FF000000000000)>>40) |
                ((num & 0x0000FF0000000000)>>24) |
                ((num & 0x000000FF00000000)>>8)  |
                ((num & 0x00000000FF000000)<<8)  |
                ((num & 0x0000000000FF0000)<<24) |
                ((num & 0x000000000000FF00)<<40) |
                ((num & 0x00000000000000FF)<<56) ;
    #endif
}

CFUNCTION void pswap_16b(void* num)
{
    uint16_t unsint = 0;
    memcpy(&unsint, num, sizeof(unsint));

    unsint = swap_16b(unsint);
    memcpy(num, &unsint, sizeof(unsint));
}

CFUNCTION void pswap_32b(void* num)
{
    uint32_t unsint = 0;
    memcpy(&unsint, num, sizeof(unsint));

    unsint = swap_32b(unsint);
    memcpy(num, &unsint, sizeof(unsint));
}

CFUNCTION void pswap_64b(void* num)
{
    uint64_t unsint = 0;
    memcpy(&unsint, num, sizeof(unsint));

    unsint = swap_64b(unsint);
    memcpy(num, &unsint, sizeof(unsint));
}

CFUNCTION const char* endianness_getname()
{
    #if ENDIANNESS == ORDER_LE
        const char* ordername = "little-endian";
    #elif ENDIANNESS == ORDER_BE
        const char* ordername = "big-endian";
    #elif ENDIANNESS == ORDER_PDP
        const char* ordername = "pdp-endian";
    #else
        const char* ordername = "unknown";
    #endif

    return ordername;
}