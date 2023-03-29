## [<- Back to the main page](../Main.md)

## Bytes-swapping functions ([endianness.h](../../endianness.h))

### <code>uint16_t swap_16b(uint16_t num)</code>
Swap 2 bytes in specified number.

*For example: 0123<sub>16</sub> becomes 2301<sub>16</sub>*.

### <code>uint32_t swap_32b(uint32_t num)</code>
Swap 4 bytes in specified number.

*For example: 01234567<sub>16</sub> becomes 67452301<sub>16</sub>*.

### <code>uint64_t swap_64b(uint64_t num)</code>
Swap 8 bytes in specified number.

*For example: 0123456789ABCDEF<sub>16</sub> becomes EFCDAB8967452301<sub>16</sub>*.

### <code>void pswap_16b(void* num)</code>
Swap 2 bytes by specified address.

Check [swap_16b](#uint16_t-swap_16buint16_t-num) for example.

### <code>void pswap_32b(void* num)</code>
Swap 4 bytes by specified address.

Check [swap_32b](#uint32_t-swap_32buint32_t-num) for example.

### <code>void pswap_64b(void* num)</code>
Swap 8 bytes by specified address.

Check [swap_64b](#uint64_t-swap_64buint64_t-num) for example.