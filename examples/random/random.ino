/**
 * @file    random.ino
 * @brief   Demonstrates the trng library on Arduino UNO R4.
 */
#include <trng.h>

/**
 * @brief  Initialize serial and TRNG hardware; halts on failure.
 */
void setup() {
    Serial.begin(115200);
    while (!Serial);

    if (!TRNG.begin()) {
        Serial.println("TRNG init failed!");
        while (1);
    }
    Serial.println("TRNG ready.\n");
}

/**
 * @brief  Print random values of various sizes every 1 second.
 */
void loop() {
    /* 128-bit hex string */
    uint32_t rnd[4];
    if (TRNG.read128(rnd)) {
        char s[33] = {0};
        sprintf(s, "%08lX%08lX%08lX%08lX",
            (unsigned long)rnd[0], (unsigned long)rnd[1],
            (unsigned long)rnd[2], (unsigned long)rnd[3]);
        Serial.print("128-bit : ");
        Serial.println(s);
    }

    /* Single values */
    uint32_t v32;
    uint16_t v16;
    uint8_t  v8;

    if (TRNG.random32(&v32)) {
        Serial.print("32-bit  : ");
        Serial.println(v32, HEX);
    }
    if (TRNG.random16(&v16)) {
        Serial.print("16-bit  : ");
        Serial.println(v16, HEX);
    }
    if (TRNG.random8(&v8)) {
        Serial.print("8-bit   : ");
        Serial.println(v8, HEX);
    }

    /* Range */
    uint32_t dice;
    if (TRNG.randomRange(&dice, 1, 6)) {
        Serial.print("Dice    : ");
        Serial.println(dice);
    }

    /* Fill buffer */
    uint8_t key[32];
    if (TRNG.fillRandom(key, sizeof(key))) {
        Serial.print("Key     : ");
        for (int i = 0; i < 32; i++) {
            char hex[3];
            sprintf(hex, "%02X", key[i]);
            Serial.print(hex);
        }
        Serial.println();
    }

    Serial.println();
    delay(1000);
}
