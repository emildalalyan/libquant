#include "math.h"

/* Interpolation =============== */

CFUNCTION double math_interpolate(interp_method method, double y0, double y1, double pos)
{
    if(y0 == y1 || pos == 0) return y0;
    
    switch(method)
    {
        case INTERPOLATION_NEAREST:
        {
            if(pos < 0.5) return y0;
            else return y1;
        }
        case INTERPOLATION_LINEAR:
        {
            return y0+((y1-y0)*pos);
        }
        case INTERPOLATION_SINE:
        {
            double sine = sin(MATH_PI*pos/2.0);

            return y0+((y1-y0)*(sine*sine));
        }
    }
}

/* ============================= */

/* Logarithm =================== */

CFUNCTION double math_logbase(double num, double base)
{
    return (log2(num)/log2(base));
    // We use logarithm property: log_a(b) = log_c(b)/log_c(a).
}

/* ============================= */