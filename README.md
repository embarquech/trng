# trng

Hardware True Random Number Generator library for **Arduino UNO R4** (WiFi & Minima).

Uses the SCE5 TRNG peripheral on the Renesas RA4M1 to generate cryptographically secure true random numbers.

## Quick start

```cpp
#include <trng.h>

void setup() {
    Serial.begin(115200);
    TRNG.begin();
}

void loop() {
    uint32_t val;
    if (TRNG.random32(&val)) {
        Serial.println(val, HEX);
    }
    delay(1000);
}
```

## API

All methods return `bool` (`true` on success) and write results via pointer.

| Method | Description |
|---|---|
| `TRNG.begin()` | Initialize the TRNG. |
| `TRNG.random32(uint32_t *out)` | Write a random `uint32_t` into `out`. |
| `TRNG.random16(uint16_t *out)` | Write a random `uint16_t` into `out`. |
| `TRNG.random8(uint8_t *out)` | Write a random `uint8_t` into `out`. |
| `TRNG.randomRange(uint32_t *out, min, max)` | Write a random value in [min, max] into `out`. |
| `TRNG.read128(uint32_t out[4])` | Read a full 128-bit block. |
| `TRNG.fillRandom(uint8_t *buf, size_t len)` | Fill a byte buffer with random data. |

## Documentation

Full API documentation is available at [embarquech.github.io/trng](https://embarquech.github.io/trng/).

## Compatibility

- Arduino UNO R4 WiFi
- Arduino UNO R4 Minima

## License

LGPL-2.1 — See [LICENSE](LICENSE) for details.

This library depends on Renesas FSP (BSD-3-Clause).
