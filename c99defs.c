#include "build.h"

#if !defined(SUPPORTS_C99) && !defined(SUPPORTS_CPP11)
// These functions implementations should exist ONLY
// if standard library doesn't have them.
// Otherwise, compilers confuse with more than
// one implementation of the one function.

    #include "math.h"

    double log2(double x)
    {
        return log10(x)*MATH_LOG2_10;
        // Logarithm properties, using by this function:
        // 1) log_a(b) = log_c(b)/log_c(a)
        // 2) log_a(b) = 1/log_b(a)
    }

    double fmin(double x, double y)
    {
        return (x > y) ? x : y;
        // This implementation is not fastest,
        // but it's portable.
    }

#endif