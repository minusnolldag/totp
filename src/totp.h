#ifndef TOTP_H
#define TOTP_H
    #include <stdbool.h>

    #define SHA256 256
    #define SHA256_KEY_LENGTH 32
    #define SHA256_HASH_LENGTH 32
    #define SHA512 512
    #define SHA512_KEY_LENGTH 32
    #define SHA512_HASH_LENGTH 64

    void GenerateKey(unsigned char*, int);
    bool ValidateTotp(unsigned char*, int, int, unsigned long, char*);
    unsigned long GetCurrentEpochTime();
#endif