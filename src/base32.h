#ifndef BASE32_H
#define BASE32_H
    #define ENCODED_BASE_32_LENGTH(a) (int)ceil((float)a / 5) * 8
    #define DECODED_BASE_32_LENGTH(a) (int)floor((float)a * 5 / 8)

    void EncodeUnsignedCharArrayToBase32StringArray(unsigned char*, int, unsigned char*, int);
    void DecodeBase32CharArrayToUnsignedCharArray(unsigned char*, int, unsigned char*, int);
#endif