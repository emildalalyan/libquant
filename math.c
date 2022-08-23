#include "math.h"

CFUNCTION double math_logbase(double num, double base)
{
    return (log2(num)/log2(base));
    // We use logarithm property: log_a(b) = log_c(b)/log_c(a).
}