## [<- Back to the main page](../Main.md)

## Math definitions ([math.h](../../math.h))

### <code>interp_method</code>
Alias for <code>enum interp_method</code>. It includes following items:
- <code>INTERPOLATION_NEAREST</code>: **Nearest-neighbor** method.

    It is the simplest interpolation method.
    Intermediate values equals nearest ones.
    
- <code>INTERPOLATION_LINEAR</code>: **Linear interpolation** method.

    It has following formula: $y = y_{0}+((y_{1}-y_{0})\times(x-x_{0}/x_{1}-x_{0}))$
    
    *Link on Wikipedia is [here](https://en.wikipedia.org/wiki/Linear_interpolation)*

- <code>INTERPOLATION_SINE</code>: **Sine interpolation** method (also called cosine interpolation method).

    Function $\sin^2(\pi x / 2)$ in range $[0,1]$ has values between 0 and 1, but it's changing non-linearly.

    Sine interpolation is linear interpolation, where $(x-x_{0})/(x_{1}-x_{0})$ has been replaced by $\sin^2(\pi(x-x_{0}/x_{1}-x_{0})/2)$.

    It also called cosine interpolation because $\sin^2(\pi x/2) = (1-\cos(\pi x))/2$.

### Math constants

- <code>MATH_PI = 3.1415926535897932</code>: [Pi constant](https://en.wikipedia.org/wiki/Pi). 
- <code>MATH_E = 2.71828</code>: [Euler's number](https://en.wikipedia.org/wiki/E_(mathematical_constant))
- <code>MATH_LOG2_10 = 3.32193</code>: Computed $log_{2} 10$ value.