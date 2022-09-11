# Time-based One-time Password implementation in C
Time-based One-time Password (TOTP) is based on the HMAC-based One-Time Password (HOTP) algorithm which is extended to support the time based input, SHA-256 and SHA-512.

**NOTE:** This repository uses [libsodium](https://github.com/jedisct1/libsodium) to calculate the HMAC checksum which **does not support** SHA-1!

## Usage
TOTP-HMAC-SHA-256
```
cd examples
gcc -o totp-256 example-256.c ../src/base32.c ../src/totp.c -lsodium
```

TOTP-HMAC-SHA-512
```
cd examples
gcc -o totp-512 example-512.c ../src/base32.c ../src/totp.c -lsodium
```