## [<- Back to the main page](../Main.md)

## Embedded math functions ([math.h](../../math.h))

### <code>double math_sinc(double x)</code>
Calculates unnormalized [cardinal sine](https://en.wikipedia.org/wiki/Sinc_function) function.

Its formula is $\sin(x)/x$ if $x$ is not 0, otherwise it's 1 by definition.

### <code>double math_logbase(double num, double base)</code>

- <code>base</code>: Base of the logarithm. <u>It cannot be cannot be 1, 0, or negative.</u>
- <code>num</code>: Number, which logarithm wanted to be calculated.

Calculate logarithm of the number to specified base.

### <code>double math_interpolate(interp_method method, double y0, double y1, double pos)</code>

- <code>method</code>: Interpolation method (see <code>[interp_method](../defs/math.md#interp_method)</code>)
- <code>y0</code>: Initial value.
- <code>y1</code>: Finite value.
- <code>pos</code>: Position of the desired value. You can calculate it using following formula: $(x-x_{0})/(x_{1}-x_{0})$. (see [Wikipedia](https://en.wikipedia.org/wiki/Linear_interpolation) for explanation).

Find intermediate value of the function with interpolation.