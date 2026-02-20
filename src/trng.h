/*******************************************************************************
 * @file    trng.h
 * @brief   Hardware True Random Number Generator library for Arduino UNO R4.
 *
 * Provides a simple interface to the SCE5 TRNG peripheral on the Renesas
 * RA4M1 MCU. Generates cryptographically secure true random numbers using
 * dedicated hardware.
 *
 * @note    Only compatible with Arduino UNO R4 WiFi and R4 Minima.
 * @license LGPL-2.1
 *
 * @example random.ino
 ******************************************************************************/
#ifndef TRNG_H
#define TRNG_H

#include <stdint.h>
#include <stddef.h>

/** @brief Success return code. */
#define TRNG_OK     0U
/** @brief Failure return code. */
#define TRNG_NOK    1U

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Initialize the SCE5 TRNG peripheral.
 *
 * Powers on the SCE5 engine and performs MCU-specific initialization.
 * Must be called once before any other trng function.
 *
 * @retval  0   Success.
 * @retval  1   Initialization failed.
 */
uint8_t trng_begin(void);

/**
 * @brief   Read 128 bits (4 x 32-bit words) of true random data.
 *
 * @param[out] out  Pointer to an array of at least 4 uint32_t.
 *
 * @retval  0   Success.
 * @retval  1   Read failed or not initialized.
 */
uint8_t trng_read128(uint32_t *out);

/**
 * @brief   Generate a single 32-bit true random number.
 *
 * @param[out] out  Pointer to a uint32_t.
 *
 * @retval  0   Success.
 * @retval  1   Read failed or not initialized.
 */
uint8_t trng_random32(uint32_t *out);

/**
 * @brief   Generate a single 16-bit true random number.
 *
 * @param[out] out  Pointer to a uint16_t.
 *
 * @retval  0   Success.
 * @retval  1   Read failed or not initialized.
 */
uint8_t trng_random16(uint16_t *out);

/**
 * @brief   Generate a single 8-bit true random number.
 *
 * @param[out] out  Pointer to a uint8_t.
 *
 * @retval  0   Success.
 * @retval  1   Read failed or not initialized.
 */
uint8_t trng_random8(uint8_t *out);

/**
 * @brief   Generate a random number within a range [min, max].
 *
 * @param[out] out  Pointer to a uint32_t.
 * @param      min  Minimum value (inclusive).
 * @param      max  Maximum value (inclusive, must be > min).
 *
 * @retval  0   Success.
 * @retval  1   Read failed, not initialized, or min >= max.
 * @note    Subject to modulo bias when (max - min + 1) is not a power of 2.
 */
uint8_t trng_randomRange(uint32_t *out, uint32_t min, uint32_t max);

/**
 * @brief   Fill a buffer with true random bytes.
 *
 * @param[out] buf   Pointer to the output buffer.
 * @param      len   Number of bytes to fill.
 *
 * @retval  0   Success.
 * @retval  1   Read failed or not initialized.
 */
uint8_t trng_fillRandom(uint8_t *buf, size_t len);

#ifdef __cplusplus
}
#endif

/* ---- C++ wrapper class ---- */
#ifdef __cplusplus

/**
 * @class   trngClass
 * @brief   C++ wrapper for the R4 hardware TRNG.
 *
 * Usage:
 * @code
 *   #include <trng.h>
 *
 *   void setup() {
 *       TRNG.begin();
 *       uint32_t val;
 *       TRNG.random32(&val);
 *   }
 * @endcode
 */
class trngClass {
public:
    /** @brief Initialize the TRNG. @return true on success. */
    bool begin()                                    { return trng_begin() == TRNG_OK; }
    /** @brief Read 128 bits into a 4-element uint32_t array. */
    bool read128(uint32_t *out)                     { return trng_read128(out) == TRNG_OK; }
    /** @brief Write a random 32-bit value into @p out. */
    bool random32(uint32_t *out)                    { return trng_random32(out) == TRNG_OK; }
    /** @brief Write a random 16-bit value into @p out. */
    bool random16(uint16_t *out)                    { return trng_random16(out) == TRNG_OK; }
    /** @brief Write a random 8-bit value into @p out. */
    bool random8(uint8_t *out)                      { return trng_random8(out) == TRNG_OK; }
    /** @brief Write a random value in [min, max] into @p out. */
    bool randomRange(uint32_t *out, uint32_t mn, uint32_t mx)
                                                    { return trng_randomRange(out, mn, mx) == TRNG_OK; }
    /** @brief Fill a buffer with random bytes. */
    bool fillRandom(uint8_t *buf, size_t len)       { return trng_fillRandom(buf, len) == TRNG_OK; }
};

/** @brief Global TRNG instance. */
inline trngClass TRNG;

#endif /* __cplusplus */
#endif /* TRNG_H */
