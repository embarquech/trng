/*******************************************************************************
 * @file    trng.c
 * @brief   Hardware True Random Number Generator library for Arduino UNO R4.
 *
 * Implements the TRNG API using the SCE5 peripheral on the Renesas RA4M1 MCU.
 * Generates cryptographically secure true random numbers via dedicated hardware.
 *
 * Compiled as C to avoid conflicts with Renesas FSP headers that use
 * C++ reserved keywords ("private", "public") as struct field names.
 *
 * @note    Only compatible with Arduino UNO R4 WiFi and R4 Minima.
 * @license LGPL-3.0
 ******************************************************************************/
#include "trng.h"
#include <hw_sce_private.h>
#include <hw_sce_trng_private.h>

/** @brief Tracks initialization state (0 = not ready, 1 = ready). */
static uint8_t _initialized = 0U;

/**
 * @brief  Initialize the SCE hardware for TRNG use.
 * @retval TRNG_OK   Success.
 * @retval TRNG_NOK  Initialization failed.
 */
// cppcheck-suppress unusedFunction
uint8_t trng_begin(void) {
    uint8_t result = TRNG_NOK;

    HW_SCE_PowerOn();

    fsp_err_t err = HW_SCE_McuSpecificInit();
    if (err == FSP_SUCCESS) {
        _initialized = 1U;
        result = TRNG_OK;
    }

    return result;
}

/**
 * @brief  Read 128 bits of true random data.
 * @param[out] out  Buffer of at least 4 uint32_t.
 * @retval TRNG_OK   Success.
 * @retval TRNG_NOK  Read failed or not initialized.
 */
uint8_t trng_read128(uint32_t *out) {
    uint8_t result = TRNG_NOK;

    if ((_initialized != 0U) && (out != NULL)) {
        if (HW_SCE_RNG_Read(out) == FSP_SUCCESS) {
            result = TRNG_OK;
        }
    }

    return result;
}

/**
 * @brief  Write a single 32-bit random value into @p out.
 * @param[out] out  Pointer to a uint32_t.
 * @retval TRNG_OK   Success.
 * @retval TRNG_NOK  Read failed or not initialized.
 */
uint8_t trng_random32(uint32_t *out) {
    uint8_t result = TRNG_NOK;

    if (out != NULL) {
        uint32_t buf[4U];
        if (trng_read128(buf) == TRNG_OK) {
            *out = buf[0U];
            result = TRNG_OK;
        }
    }

    return result;
}

/**
 * @brief  Write a single 16-bit random value into @p out.
 * @param[out] out  Pointer to a uint16_t.
 * @retval TRNG_OK   Success.
 * @retval TRNG_NOK  Read failed or not initialized.
 */
// cppcheck-suppress unusedFunction
uint8_t trng_random16(uint16_t *out) {
    uint8_t result = TRNG_NOK;

    if (out != NULL) {
        uint32_t val;
        if (trng_random32(&val) == TRNG_OK) {
            *out = (uint16_t)(val & 0xFFFFU);
            result = TRNG_OK;
        }
    }

    return result;
}

/**
 * @brief  Write a single 8-bit random value into @p out.
 * @param[out] out  Pointer to a uint8_t.
 * @retval TRNG_OK   Success.
 * @retval TRNG_NOK  Read failed or not initialized.
 */
// cppcheck-suppress unusedFunction
uint8_t trng_random8(uint8_t *out) {
    uint8_t result = TRNG_NOK;

    if (out != NULL) {
        uint32_t val;
        if (trng_random32(&val) == TRNG_OK) {
            *out = (uint8_t)(val & 0xFFU);
            result = TRNG_OK;
        }
    }

    return result;
}

/**
 * @brief  Write a random value in [min, max] (inclusive) into @p out.
 * @param[out] out  Pointer to a uint32_t.
 * @param  min  Lower bound.
 * @param  max  Upper bound (must be >= min).
 * @retval TRNG_OK   Success.
 * @retval TRNG_NOK  Read failed, not initialized, or min > max.
 * @note   Uses rejection sampling to eliminate modulo bias.
 */
// cppcheck-suppress unusedFunction
uint8_t trng_randomRange(uint32_t *out, uint32_t min, uint32_t max) {
    uint8_t result = TRNG_NOK;

    if ((out != NULL) && (min <= max)) {
        uint32_t range = (max - min) + 1U;

        // cppcheck-suppress knownConditionTrueFalse ; unsigned overflow possible
        if (range == 0U) {
            /* Full 32-bit range (overflow): any value is valid. */
            result = trng_random32(out);
        } else {
            uint32_t val;
            uint32_t threshold = (UINT32_MAX - range + 1U) % range;
            uint8_t err = 0U;
            uint8_t found = 0U;

            while ((found == 0U) && (err == 0U)) {
                if (trng_random32(&val) == TRNG_OK) {
                    if (val >= threshold) {
                        *out = min + (val % range);
                        result = TRNG_OK;
                        found = 1U;
                    }
                } else {
                    err = 1U;
                }
            }
        }
    }

    return result;
}

/**
 * @brief  Fill a buffer with random bytes.
 * @param[out] buf  Destination buffer.
 * @param      len  Number of bytes to fill.
 * @retval TRNG_OK   Success.
 * @retval TRNG_NOK  Read failed or not initialized.
 */
// cppcheck-suppress unusedFunction
uint8_t trng_fillRandom(uint8_t *buf, size_t len) {
    uint8_t result = TRNG_NOK;

    if ((_initialized != 0U) && (buf != NULL)) {
        uint32_t tmp[4U];
        size_t i = 0U;
        result = TRNG_OK;

        while ((i < len) && (result == TRNG_OK)) {
            if (HW_SCE_RNG_Read(tmp) != FSP_SUCCESS) {
                result = TRNG_NOK;
            } else {
                const uint8_t *src = (const uint8_t *)tmp;
                size_t chunk = ((len - i) < 16U) ? (len - i) : 16U;
                size_t j;
                for (j = 0U; j < chunk; j++) {
                    buf[i] = src[j];
                    i++;
                }
            }
        }
    }

    return result;
}
