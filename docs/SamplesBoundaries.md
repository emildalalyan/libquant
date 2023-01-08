## [<- Back to the main page](Main.md)

## 24-bit integer and slevel_t boundaries ([samples.h](../samples.h))

*samples.h* has some definitions, that can help in case, when you need to know limits of [<code>slevel_t</code>](Architecture.md#library-architecture) or 24-bit integer.

### <code>SLEVEL_MIN</code>
### <code>SLEVEL_MAX</code>
Minimum and maximum possible values, that may be stored in [<code>slevel_t</code>](Architecture.md#library-architecture).

### <code>SLEVEL_BIT_DEPTH</code>
Length of [<code>slevel_t</code>](Architecture.md#library-architecture) integer (in bits).

### <code>SLEVEL_FORMAT</code>
Format, that can be in function like <code>printf</code> for displaying [<code>slevel_t</code>](Architecture.md#library-architecture) value.

### 24-bit integer limits

### <code>INT24_MIN</code>
### <code>INT24_MAX</code>
Represents range of the values for 24-bit signed integer.

### <code>UINT24_MIN</code>
### <code>UINT24_MAX</code>
Represents range of the values for 24-bit unsigned integer.
