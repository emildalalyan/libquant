#include "math.h"

/* Interpolation =============== */

CFUNCTION double math_interpolate(interp_method method, double y0, double y1, double pos)
{
    if(y0 == y1 || pos == 0) return y0;
    // In this cases any interpolation method will return y0.
    
    switch(method)
    {
        case INTERPOLATION_LINEAR:
        {
            return y0+((y1-y0)*pos);
        }
        case INTERPOLATION_SINE:
        {
            double sine = sin(MATH_PI*pos/2.0);

            return y0+((y1-y0)*(sine*sine));
        }
        
        default:
        case INTERPOLATION_NEAREST:
        {
            if(pos < 0.5) return y0;
            else return y1;
        }
    }
}

/* ============================= */

/* Logarithm =================== */

CFUNCTION double math_logbase(double num, double base)
{
    if(base == 1) return FUNC_INVALID_ARG;

    return (log2(num)/log2(base));
    // We use logarithm property: log_a(b) = log_c(b)/log_c(a).
}

/* ============================= */

/* Trigonometry ================ */

CFUNCTION double math_sinc(double x)
{
    if(x == 0) return 1;
    // sinc(0) is 1 by definition.
    // It's defined as the limit (x->0) of sin(x)/x function.

    return sin(x)/x;
}

/* ============================= */