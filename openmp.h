#pragma once

#include <stddef.h>
#include "build.h"

#if !defined(_OPENMP)
// If OpenMP support is disabled, then we should declare some
// OpenMP functions, to work without OpenMP support.

    COMPILER_WARNING("OpenMP is disabled! Performance may be reduced on multi-processor systems.")
    // If OpenMP support is disabled, then we pass all loops in one thread.

    static int omp_get_thread_num(void)
    {
        return 0;
        // If OpenMP support is disabled, then we're
        // working only in one thread (0th thread).
    }

    static int omp_get_max_threads(void)
    {
        return 1;
        // If OpenMP support is disabled, then we're
        // working in one thread.
    }

    /* Iterator type for loops, that use OpenMP for parallelizing */
    typedef size_t omp_iter_t;
#else
    #include <omp.h>

    #if _OPENMP >= 200805
        /* Iterator type for loops, that use OpenMP for parallelizing */
        typedef size_t omp_iter_t;
    #else
        /* Iterator type for loops, that use OpenMP for parallelizing */
        typedef long long omp_iter_t;

        COMPILER_WARNING("This compiler does not support OpenMP 3.0. Iterator type will be long long.")
        // Unsigned iterator type support was introduced with OpenMP 3.0.
        // Old compilers, that supports only OpenMP 2.0, will compile this code,
        // but maximum iteration number will be restricted to long long maximum.
    #endif
#endif