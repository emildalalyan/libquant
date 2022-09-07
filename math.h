#pragma once

#include <math.h>
#include "build.h"

#define MATH_PI 3.1415926535897932
#define MATH_E  2.71828

/* Calculate the "base" logarithm of number "num". */
CFUNCTION double math_logbase(double num, double base);