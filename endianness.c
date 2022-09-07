#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "endianness.h"

CFUNCTION uint16_t swap_16b(uint16_t num)
{
    #if (defined(ARCH_X86) || defined(ARCH_X86_64)) && defined(SUPPORTS_GCC)
        uint16_t result = 0;
        __asm__("xchg %%al, %%ah" : "=a"(result) : "a"(num));

        return result;
    #elif defined(ARCH_X86) && defined(SUPPORTS_MSVC)
        uint16_t result = 0;
        __asm
        {
            mov ax, num
            xchg al, ah
            mov result, ax
        }

        return result;
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
        __asm__("bswap %%eax\n"
                "bswap %%edx" : "=a"(res_high), "=d"(res_low) : "a"(res_high), "d"(res_low));

        return (((uint64_t)res_low << 32) | (uint64_t)res_high);
    #elif defined(ARCH_X86) && defined(SUPPORTS_MSVC)
        uint32_t res_high = 0, res_low = 0;
        res_high = (num >> 32) & UINT32_MAX;
        res_low = num & UINT32_MAX;
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

CFUNCTION int16_t iswap_16b(int16_t num)
{
    uint16_t unsint = 0;
    memcpy(&unsint, &num, sizeof(uint16_t));

    unsint = swap_16b(unsint);
    memcpy(&num, &unsint, sizeof(uint16_t));

    return num;
}

CFUNCTION int32_t iswap_32b(int32_t num)
{
    uint32_t unsint = 0;
    memcpy(&unsint, &num, sizeof(uint32_t));

    unsint = swap_32b(unsint);
    memcpy(&num, &unsint, sizeof(uint32_t));

    return num;
}

CFUNCTION int64_t iswap_64b(int64_t num)
{
    uint64_t unsint = 0;
    memcpy(&unsint, &num, sizeof(uint64_t));

    unsint = swap_64b(unsint);
    memcpy(&num, &unsint, sizeof(uint64_t));

    return num;
}

CFUNCTION float fswap_32b(float num)
{
    uint32_t intnum = 0;
    memcpy(&intnum, &num, sizeof(float));

    intnum = swap_32b(intnum);
    memcpy(&num, &intnum, sizeof(float));

    return num;
}

CFUNCTION double fswap_64b(double num)
{
    uint64_t intnum = 0;
    memcpy(&intnum, &num, sizeof(double));

    intnum = swap_64b(intnum);
    memcpy(&num, &intnum, sizeof(double));

    return num;
}

CFUNCTION const char* endianness_getname()
{
    #if ENDIANNESS == __ORDER_LITTLE_ENDIAN__
        const char* endiannessname = "little-endian";
    #elif ENDIANNESS == __ORDER_BIG_ENDIAN__
        const char* endiannessname = "big-endian";
    #elif ENDIANNESS == __ORDER_PDP_ENDIAN__
        const char* endiannessname = "pdp-endian";
    #else
        const char* endiannessname = "unknown";
    #endif

    return endiannessname;
}