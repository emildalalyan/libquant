#pragma once

#if defined(__cplusplus)
   #include <cmath>
   // <math.h> usage is considered deprecated in C++.
   // Some compilers (such as old Microsoft Visual C++)
   // doesn't even contain <math.h>.
   // So we use standard <cmath> header.
#else
   #include <math.h>
#endif

#include "build.h"

/* Math constants ============== */

#define MATH_PI 3.1415926535897932
#define MATH_E  2.71828

#define MATH_LOG2_10 3.32193

/* ============================= */

/* Interpolation =============== */

/* Enumeration of possible interpolation methods,
   i.e intermediate values finding methods. */
typedef enum interp_method
{
   /* Nearest-neighbor method.
      It is the simplest interpolation method.
      Intermediate values equals nearest ones. */
   INTERPOLATION_NEAREST = 0,

   /* Linear interpolation method.
      It has following formula: y = y0+((y1-y0)*(x-x0/x1-x0)), where
      y0 - is beginning value, x0 - its coordinate,
      y1 - is ending value, x1 - its coordinate,
      y - is desired (intermediate) value, x - its coordinate.

      (x-x0/x1-x0) is the position of x between x0 and x1,
      it's 0 when x equals x0, and it's 1 when x equals x1. */       
   INTERPOLATION_LINEAR = 1,

   /* Sine interpolation method (also called cosine interpolation method).
      Function sin^2(pi*x/2) in range [0,1] has values between 0 and 1, but it's changing non-linearly.
      Sine interpolation is linear interpolation, where (x-x0/x1-x0) has been replaced by sin^2(pi*(x-x0/x1-x0)/2).
       
      It also called cosine interpolation because sin^2(pi*x/2) = (1-cos(pi*x))/2. */
   INTERPOLATION_SINE = 2
} interp_method;

/* Find intermediate value of function with interpolation.
   First argument is the interpolation method (see enum interp_method).
   y0 is beginning value, y1 is ending value. 
   Last argument is the position of desired value between beginning and ending values. 
   The formula is: (x-x0)/(x1-x0), where x is coordinate of desired value,
   and x0, x1 is coordinates of beginning, ending values respectively. */
CFUNCTION double math_interpolate(interp_method method, double y0, double y1, double pos);

/* ============================= */

/* Logarithm =================== */

/* Calculates the "base" logarithm of number "num". */
CFUNCTION double math_logbase(double num, double base);

/* ============================= */

/* Trigonometry ================ */

/* Calculates unnormalized cardinal sine function.
   Its formula is (sin(x)/x) if x is not 0,
   otherwise it's 1 by definition. */
CFUNCTION double math_sinc(double x);

/* ============================= */