#include "base32.h"
#include <math.h>

unsigned char base32Symbols[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', '2', '3', '4', '5', '6', '7'
};

/*
* Encodes unsigned char array to Base32 format
*
* @param unsigned char* t - TotpHash to encode
* @param int s - Size of totpHash array
* @param unsigned char* d - Destination array
* @param int ds - Destination array size
*/
void EncodeUnsignedCharArrayToBase32StringArray(unsigned char* t, int s, unsigned char* d, int ds) {
    // Input array size is not 0
    if (s != 0) {
        int offsetCounter = 0;
        int base32Counter = 0;

        // Calculate the total encoded characters before padding to make
        // adding padding in the loop easier
        int totalWithoutPadding = ceil(((float)s * 8) / 5);

        for (int i = 0; i < ds; i++) {
            // Pad the remaining characters in the array after decoding every character
            if (i >= totalWithoutPadding) {
                d[i] = '=';
            } else {
                int v = 0;

                // 8 individual steps to take 5 bits at different offsets
                if (base32Counter == 0) {
                    v = t[offsetCounter] >> 3;
                } else if (base32Counter == 1) {
                    v = (t[offsetCounter] & 7) << 2;
                    offsetCounter++;

                    if (offsetCounter < s) {
                        v += t[offsetCounter] >> 6;
                    }
                } else if (base32Counter == 2) {
                    v = (t[offsetCounter] & 63) >> 1;
                } else if (base32Counter == 3) {
                    v = (t[offsetCounter] & 1) << 4;
                    offsetCounter++;

                    if (offsetCounter < s) {
                        v += t[offsetCounter] >> 4;
                    }
                } else if (base32Counter == 4) {
                    v = (t[offsetCounter] & 15) << 1;
                    offsetCounter++;

                    if (offsetCounter < s) {
                        v += t[offsetCounter] >> 7;
                    }
                } else if (base32Counter == 5) {
                    v = (t[offsetCounter] & 127) >> 2;
                } else if (base32Counter == 6) {
                    v = (t[offsetCounter] & 3) << 3;
                    offsetCounter++;

                    if (offsetCounter < s) {
                        v += t[offsetCounter] >> 5;
                    }
                } else if (base32Counter == 7) {
                    v = t[offsetCounter] & 31;
                    offsetCounter++;
                }

                // Set the encoded character to the destination array
                d[i] = base32Symbols[v];

                // Once it has done the 8 individual steps start again
                if (base32Counter == 7) {
                    base32Counter = 0;
                } else {
                    base32Counter++;
                }
            }
        }
    }
}

/*
* Decodes a Base32 array to a destination unsigned char* array
*
* @param unsigned char* t - TotpHash to decode
* @param int s - Size of totpHash array
* @param unsigned char* d - Destination array
* @param int ds - Destination array size
*/
void DecodeBase32CharArrayToUnsignedCharArray(unsigned char* t, int s, unsigned char* d, int ds) {
    // Input array size is not 0
    if (s != 0) {
        int offsetCounter = 0;
        int base32Counter = 0;

        // Set array to 0
        for (int i = 0; i < ds; i++) {
            d[i] = 0;
        }

        for (int i = 0; i < s; i++) {
            if (offsetCounter < ds) {
                // Do not decode = characters
                if (t[i] != 61) {
                    unsigned int a = 0;

                    if (t[i] >= 50 && t[i] <= 55) {
                        // If the character is a number
                        a = t[i] - (unsigned int)24;
                    } else {
                        // If the character is a letter
                        a = t[i] - (unsigned int)65;
                    }

                    // 8 individual steps to take a char and convert to 5 bits at different offsets
                    if (base32Counter == 0) {
                        d[offsetCounter] = a << 3;
                    } else if (base32Counter == 1) {
                        d[offsetCounter] += a >> 2;
                        offsetCounter++;

                        if (offsetCounter < ds) {
                            d[offsetCounter] = (a & 3) << 6;
                        }
                    } else if (base32Counter == 2) {
                        d[offsetCounter] += a << 1;
                    } else if (base32Counter == 3) {
                        d[offsetCounter] += a >> 4;
                        offsetCounter++;

                        if (offsetCounter < ds) {
                            d[offsetCounter] = (a & 15) << 4;
                        }
                    } else if (base32Counter == 4) {
                        d[offsetCounter] += a >> 1;
                        offsetCounter++;

                        if (offsetCounter < ds) {
                            d[offsetCounter] = (a & 1) << 7;
                        }
                    } else if (base32Counter == 5) {
                        d[offsetCounter] += a << 2;
                    } else if (base32Counter == 6) {
                        d[offsetCounter] += a >> 3;
                        offsetCounter++;

                        if (offsetCounter < ds) {
                            d[offsetCounter] = (a & 7) << 5;
                        }
                    } else if (base32Counter == 7) {
                        d[offsetCounter] += a;
                        offsetCounter++;
                    }
                }

                base32Counter++;

                // Once it has done the 8 individual steps start again
                if (base32Counter == 8) {
                    base32Counter = 0;
                }
            }
        }
    }
}