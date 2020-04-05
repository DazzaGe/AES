/*
        AES
*/

#ifndef AES_H
#define AES_H

#include <stddef.h>




void    AES_128_Encrypt(unsigned char* data, size_t dataSize, unsigned char* key, size_t keySize, unsigned char* out, size_t outSize);
void    AES_128_Decrypt(unsigned char* data, size_t dataSize, unsigned char* key, size_t keySize, unsigned char* out, size_t outSize);

void    AES_192_Encrypt(unsigned char* data, size_t dataSize, unsigned char* key, size_t keySize, unsigned char* out, size_t outSize);
void    AES_192_Decrypt(unsigned char* data, size_t dataSize, unsigned char* key, size_t keySize, unsigned char* out, size_t outSize);

void    AES_256_Encrypt(unsigned char* data, size_t dataSize, unsigned char* key, size_t keySize, unsigned char* out, size_t outSize);
void    AES_256_Decrypt(unsigned char* data, size_t dataSize, unsigned char* key, size_t keySize, unsigned char* out, size_t outSize);

#endif // AES_H
