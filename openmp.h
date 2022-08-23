#pragma once

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
#else
    #include <omp.h>
#endif