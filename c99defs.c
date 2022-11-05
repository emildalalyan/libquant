#include "build.h"

#if !defined(SUPPORTS_C99) && !defined(SUPPORTS_CPP11)
// These functions implementations should exist ONLY
// if standard library doesn't have them.
// Otherwise, C99/C++11 compilers confuse with
// more than one implementation of the one function.

    #include "math.h"

    double log2(double x)
    {
        return log10(x)*MATH_LOG2_10;
    }

#endif