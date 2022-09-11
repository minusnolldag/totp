#include "totp.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "base32.h"
#include "sodium.h"

void UnsignedLongToByteString(unsigned long, unsigned char*);
void Truncate(unsigned char*, int, unsigned char*);

/*
* Generates ds of random bytes
*
* @param unsigned char** d - Destination array
* @param int ds - Destination size
*/
void GenerateKey(unsigned char* d, int ds) {
    for (int i = 0; i < ds; i++) {
        d[i] = randombytes_uniform(256);
    }
}

/*
* Work out the 6 digit totp from the count and check if it is valid
*
* @param unsigned char* t - Totp
* @param int ts - Totp size
* @param int c - Count
* @param int st - SHA type
* @param char* d - Digits
* @return bool - If inputted digits are the sames as the generated digits
*/
bool ValidateTotp(unsigned char* t, int ts, int st, unsigned long c, char* d) {
    int h;
    int k;

    if (st == SHA256) {
        h = SHA256_HASH_LENGTH;
        k = SHA256_KEY_LENGTH;
    } else if (st == SHA512) {
        h = SHA512_HASH_LENGTH;
        k = SHA512_KEY_LENGTH;
    }

    unsigned char hash[h];
    unsigned char key[k];
    unsigned char message[8] = {0};
    const unsigned char* messagePointer = &message[0];
    unsigned char totp[6] = {0};
    bool isValid = true;

    UnsignedLongToByteString(c, &message[0]);
    DecodeBase32CharArrayToUnsignedCharArray(t, ts, key, k);

    if (st == SHA256) {
        crypto_auth_hmacsha256(hash, messagePointer, 8, key);
    } else if (st == SHA512) {
        crypto_auth_hmacsha512(hash, messagePointer, 8, key);
    }
    
    Truncate(hash, h, &totp[0]);

    // Check if the inputted digits are the same as the generated ones
    for (int i = 0; i < 6; i++) {
        if (totp[i] != d[i]) {
            isValid = false;
            
            break;
        }
    }

    return isValid;
}

/*
* Convert a unsigned long to a unsigned char[8] array
*
* @param unsigned long t - Time
* @param unsigned long* d - Destination array
*/
void UnsignedLongToByteString(unsigned long t, unsigned char* d) {
    d[0] = t >> 56;
    d[1] = t >> 48;
    d[2] = t >> 40;
    d[3] = t >> 32;
    d[4] = t >> 24;
    d[5] = t >> 16;
    d[6] = t >> 8;
    d[7] = t;
}

/*
* Converts the returned totp code to 6 digits long
*
* @param unsigned car* h - SHA512 hash
* @param int hs - hash size
* @param unsigned char* - Destination array
*/
void Truncate(unsigned char* h, int hs, unsigned char* d) {
    // Get offset
    char o =  h[hs - 1] & 0xF;
    int ts = ((h[o] & 0x7f) << 24) | ((h[o + 1] & 0xff) << 16) | ((h[o + 2] & 0xff) << 8) | (h[o + 3] & 0xff);

    // Ensure 6 digits are padded to 6 digits
    int p = ts % 1000000;

    // Convert each int digits into a unsigned char array
    d[0] = floor((p % 1000000) / 100000);
    d[1] = floor((p % 100000) / 10000);
    d[2] = floor((p % 10000) / 1000);
    d[3] = floor((p % 1000) / 100);
    d[4] = floor((p % 100) / 10);
    d[5] = p % 10;
}

/*
* Gets the current epoch time
*
* @return unsigned long - Epoch time
*/
unsigned long GetCurrentEpochTime() {
    time_t epochTime;

    time(&epochTime);

    return epochTime;
}