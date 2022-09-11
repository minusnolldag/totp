// gcc -o totp-256 example-256.c ../src/base32.c ../src/totp.c -lsodium

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "../src/base32.h"
#include "../src/totp.h"
#include "sodium.h"

int main() {
    // Generate the key
    unsigned char key[SHA256_KEY_LENGTH];
    GenerateKey(key, SHA256_KEY_LENGTH);

    // Encode to Base32
    unsigned char encoded[ENCODED_BASE_32_LENGTH(SHA256_KEY_LENGTH)];
    EncodeUnsignedCharArrayToBase32StringArray(key, SHA256_KEY_LENGTH, encoded, ENCODED_BASE_32_LENGTH(SHA256_KEY_LENGTH));

    // Validate Totp
    unsigned long epochTime = GetCurrentEpochTime();
    char digits[6] = {0, 0, 0, 0, 0, 0};
    bool isValid = ValidateTotp(encoded, ENCODED_BASE_32_LENGTH(SHA256_KEY_LENGTH), SHA256, epochTime, digits);
    printf("%d\n", isValid);

    return 0;
}