## [<- Back to the main page](../Main.md)

## Functions for samples conversions ([samples.h](../samples.h))

Using of these function is recommended for converting samples to [<code>slevel_t</code>](../Architecture.md#library-architecture) or vise-versa.

## Convert <u>from</u> integer or floating-point sample.

### <code>slevel_t i16toslt(int16_t sample)</code>
This function converts 16-bit integer sample to [<code>slevel_t</code>](../Architecture.md#library-architecture) format

### <code>slevel_t i24toslt(int32_t sample)</code>
This function converts 24-bit integer sample to [<code>slevel_t</code>](../Architecture.md#library-architecture) format.

Input argument is 32-bit wide, but least significant byte will be zeroed.

### <code>slevel_t i32toslt(int32_t sample)</code>
This function converts 32-bit integer sample to [<code>slevel_t</code>](../Architecture.md#library-architecture) format

### <code>slevel_t f32toslt(float sample)</code>
This function converts 32-bit floating-point sample to [<code>slevel_t</code>](../Architecture.md#library-architecture) format.

Input sample should be in range [-1, 1], if it will be out of this range, then function will clip it.

### <code>slevel_t f64toslt(double sample)</code>
This function converts 64-bit floating-point sample to [<code>slevel_t</code>](../Architecture.md#library-architecture) format.

Input sample should be in range [-1, 1], if it will be out of this range, then function will clip it.

## Convert <u>to</u> integer or floating-point sample.

### <code>int16_t slttoi16(slevel_t sample)</code>
This function converts [<code>slevel_t</code>](../Architecture.md#library-architecture) sample to 16-bit integer format

### <code>int32_t slttoi24(slevel_t sample)</code>
This function converts [<code>slevel_t</code>](../Architecture.md#library-architecture) sample to 24-bit integer format.

Output argument is 32-bit wide, but least significant byte won't be used.

### <code>int32_t slttoi32(slevel_t sample)</code>
This function converts [<code>slevel_t</code>](../Architecture.md#library-architecture) sample to 32-bit integer format

### <code>float f32toslt(slevel_t sample)</code>
This function converts [<code>slevel_t</code>](../Architecture.md#library-architecture) sample to 32-bit floating-point format.

Output sample will be in range [-1, 1].

### <code>double f64toslt(slevel_t sample)</code>
This function converts [<code>slevel_t</code>](../Architecture.md#library-architecture) sample to 64-bit floating-point format.

Output sample will be in range [-1, 1].