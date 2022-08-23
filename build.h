#pragma once

#include "functions.h"
#include <string.h>
#include <assert.h>
#include <stdint.h>

#if defined(__cplusplus)
    #define CFUNCTION extern "C"
    // If the code is compiling as C++ code, we need to avoid
    // C++ name mangling (or name decoration).
#else
    #define CFUNCTION extern
    // But if the code is compiling as C code, we don't need to avoid
    // C++ name mangling, because compiler doesn't mangle names.
#endif

/* Compiler detection ========== */
#define COMPILER_GCC 0
#define COMPILER_CLANG 1
#define COMPILER_ICC 2
#define COMPILER_MSVC 3
#define COMPILER_UNKNOWN 4

#if !defined(DO_PRAGMA)
    #define DO_PRAGMA(x) _Pragma(#x)
    // Concatenation of the strings are doing after
    // execution of preprocessor, so we use "#x" feature
    // (get string representation of expression)
    // to bypass concatenation.
#endif

#if defined(__GNUC__)
    #if defined(__clang__)
        #define CURRENT_COMPILER COMPILER_CLANG
    #elif defined(__INTEL_COMPILER)
        #define CURRENT_COMPILER COMPILER_ICC
    #else
        #define CURRENT_COMPILER COMPILER_GCC
    #endif
    // GCC, Clang and ICC define __GNUC__ macro, so we
    // check whether it's GCC, or not.

    #define SUPPORTS_GCC 1
    // If the compiler defines __GNUC__,
    // then it supports GCC stuff (like inline assembler).

    #define COMPILER_WARNING(msg) DO_PRAGMA(GCC warning msg)
    // Warning directive are compiler-specific, so we
    // have to replace COMPILER_WARNING
    // to compiler-specific warning.
#elif defined(_MSC_VER)
    #if defined(__clang__)
        #define CURRENT_COMPILER COMPILER_CLANG
    #elif defined(__INTEL_COMPILER)
        #define CURRENT_COMPILER COMPILER_ICC
    #else
        #define CURRENT_COMPILER COMPILER_MSVC
    #endif

    #define SUPPORTS_MSVC 1
    // If the compiler defines _MSC_VER,
    // then it supports MSVC stuff (like inline assembler).

    #define _CRT_SECURE_NO_WARNINGS 1
    // We should suppress errors about non-secure functions.
    // "*_s" functions are Microsoft-specific, so we can't use it.

    #define COMPILER_WARNING(msg) DO_PRAGMA(message( msg ))
    // Warning directive are compiler-specific, so we
    // have to replace COMPILER_WARNING
    // to compiler-specific warning.
#else
    #define CURRENT_COMPILER COMPILER_UNKNOWN
    // Compiler couldn't be determined.

    #define COMPILER_WARNING(msg) 
    // If compiler is unknown, then we just
    // make this macro empty.
#endif
/* ============================= */

/* Language detection ========== */

#if defined(__cplusplus)
    #if defined(_MSVC_LANG)
        #if _MSVC_LANG >= 199711L
            #define SUPPORTS_CPP98 1
        #endif
        #if _MSVC_LANG >= 201103L
            #define SUPPORTS_CPP11 1
        #endif
    
        // Microsoft Visual C++ defines __cplusplus
        // as 199711L by default, even if compiler
        // supports C++11. But since Visual C++ 2015,
        // compiler defines _MSVC_LANG macro,
        // that contains real language standard number.
    #else
        #if __cplusplus >= 199711L
            #define SUPPORTS_CPP98 1
        #endif
        #if __cplusplus >= 201103L
            #define SUPPORTS_CPP11 1
        #endif
    #endif
#elif defined(__STDC_VERSION__)
    #if __STDC_VERSION__ >= 199901L
        #define SUPPORTS_C99 1
    #endif
    #if __STDC_VERSION__ >= 201112L
        #define SUPPORTS_C11 1
    #endif
    #if __STDC_VERSION__ >= 201710L
        #define SUPPORTS_C17 1
    #endif
#endif
// Features support detection.

#if !defined(SUPPORTS_C99) && !defined(SUPPORTS_CPP11)
    COMPILER_WARNING("This code may not compile, because compiler does not support neither C99 features nor C++11 features.")
#endif

/* ============================= */

/* Architecture detection ====== */
#if defined(_M_IX86) || defined(__i386__)
    #define ARCH_X86 1
    // 32-bit x86-based CPU.
#endif

#if defined (_M_AMD64) || defined(__x86_64__)
    #define ARCH_X86_64 1
    // 64-bit x86-based CPU (x86-64 or AMD64).
#endif

#if defined (_M_ARM) || defined(__arm__)
    #define ARCH_ARM 1
    // 32-bit ARM-based CPU.
#endif

#if defined(_M_ARM64) || defined(__aarch64__)
    #define ARCH_ARM64 1
    // 64-bit ARM-based CPU (aarch64 or arm64).
#endif

#if defined(_M_PPC) || defined(__powerpc__)
    #define ARCH_POWERPC 1
    // 32-bit PowerPC-based CPU.
#endif

#if defined(__e2k__)
    #define ARCH_E2K 1
    // 64-bit Elbrus CPU.
#endif
/* ============================= */

/* Assertions =========================== */

#if defined(SUPPORTS_C11) || defined(SUPPORTS_CPP11)
// C have static_assert beginning from C11. C++ also have this feature beginning from C++11.
    static_assert(sizeof(float)  == sizeof(uint32_t), "Single-precision FP is not 32-bit long.");
    static_assert(sizeof(double) == sizeof(uint64_t), "Double-precision FP is not 64-bit long.");
#endif
// Checking, whether floating-point numbers types meet IEEE 754 standard or not.

/* ====================================== */

/* Get name of the compiler, that was compiling this program. */
CFUNCTION const char* build_getcompilername();

/* Get program version string. Version and suffix
   will be dash-separated. */
CFUNCTION const char* build_getversion();

/* Get program author(s) string. If program have multiple
   authors, their names will be comma-separated. */
CFUNCTION const char* build_getauthors();

/* Get name of the architecture for which program was compiled. */
CFUNCTION const char* build_getarchname();