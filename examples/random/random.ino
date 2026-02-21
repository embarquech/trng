/**
 * @file    random.ino
 * @brief   Demonstrates the trng library on Arduino UNO R4.
 */
#include <trng.h>

/**
 * @brief  Initialize serial and TRNG hardware; halts on failure.
 */
void setup() {
	
    Serial.begin(115200UL);
    while (!Serial);
	
    if (!TRNG.begin()) {
        Serial.println("TRNG init failed!");
        while (1U);
    }
	
    Serial.println("TRNG ready.\n");
}

/**
 * @brief  Print random values of various sizes every 1 second.
 */
void loop() {
	
    /* 128-bit hex string */
    uint32_t rnd[4U];
    if (TRNG.read128(rnd)) {
        char s[33U] = {0};
        sprintf(s, "%08lX%08lX%08lX%08lX",
            (unsigned long)rnd[0U], (unsigned long)rnd[1U],
            (unsigned long)rnd[2U], (unsigned long)rnd[3U]);
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
    if (TRNG.randomRange(&dice, 1U, 6U)) {
        Serial.print("Dice    : ");
        Serial.println(dice);
    }
	
    /* Fill buffer */
    uint8_t key[32U];
    if (TRNG.fillRandom(key, sizeof(key))) {
        Serial.print("Key     : ");
        for (size_t i = 0U; i < sizeof(key); i++) {
            char hex[3U];
            sprintf(hex, "%02X", key[i]);
            Serial.print(hex);
        }
        Serial.println();
    }
	
    Serial.println();
    delay(1000UL);
}
