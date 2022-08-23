#include "build.h"

CFUNCTION const char* build_getcompilername()
{
    #if CURRENT_COMPILER == COMPILER_GCC
        const char* name = "GCC";
    #elif CURRENT_COMPILER == COMPILER_ICC
        const char* name = "Intel C++ Compiler";
    #elif CURRENT_COMPILER == COMPILER_CLANG
        const char* name = "Clang";
    #elif CURRENT_COMPILER == COMPILER_MSVC
        const char* name = "Microsoft Visual C++";
    #else
        const char* name = "Unknown compiler";
    #endif

    return name;
}

CFUNCTION const char* build_getversion()
{
    const char* version = "1.0-prealpha";

    return version;
}

CFUNCTION const char* build_getauthors()
{
    const char* authors = "Emil Dalalyan";
    
    return authors;
}

CFUNCTION const char* build_getarchname()
{
    #if defined(ARCH_X86)
        const char* archname = "x86";
    #elif defined(ARCH_X86_64)
        const char* archname = "amd64";
    #elif defined(ARCH_ARM)
        const char* archname = "arm";
    #elif defined(ARCH_ARM64)
        const char* archname = "arm64";
    #elif defined(ARCH_POWERPC)
        const char* archname = "powerpc";
    #elif defined(ARCH_E2K)
        const char* archname = "e2k";
    #else
        const char* archname = "unknown";
    #endif

    return archname;
}