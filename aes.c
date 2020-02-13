/*
        AES

        This file manages encryption/decryption for AES
*/

#include "aes.h"
#include "rijndael.h"
#include "string.h"
#include "stdlib.h"




void AES_Encrypt(unsigned char* data, size_t dataSize, unsigned char* key, unsigned int Nk, unsigned int Nr, unsigned char* out, size_t outSize)
{
    uint8_t state[Nb][4];
    uint32_t* keySchedule;

    keySchedule = (uint32_t*)malloc(4 * Nb * (Nr + 1));

    for (unsigned int i = 0; i < dataSize; i += Nb * 4)
    {
        memcpy((uint8_t*)state, data + i, Nb * 4);

        Rijndael_KeyExpansion((uint32_t*)key, keySchedule, Nk, Nr);
        Rijndael_Cipher(state, keySchedule, Nr);

        memcpy(out + i, (uint8_t*)state, Nb * 4);
    }

    free(keySchedule);
}

void AES_Decrypt(unsigned char* data, size_t dataSize, unsigned char* key, unsigned int Nk, unsigned int Nr, unsigned char* out, size_t outSize)
{
}


void AES_128_Encrypt(unsigned char* data, size_t dataSize, unsigned char* key, size_t keySize, unsigned char* out, size_t outSize)
{
    if (keySize != 16 || dataSize > outSize)
        return;

    AES_Encrypt(data, dataSize, key, 4, 10, out, outSize);
}

void AES_128_Decrypt(unsigned char* data, size_t dataSize, unsigned char* key, size_t keySize, unsigned char* out, size_t outSize)
{
    if (keySize != 16 || dataSize > outSize)
        return;

    AES_Decrypt(data, dataSize, key, 4, 10, out, outSize);
}


void AES_192_Encrypt(unsigned char* data, size_t dataSize, unsigned char* key, size_t keySize, unsigned char* out, size_t outSize)
{
    if (keySize != 24 || dataSize > outSize)
        return;

    AES_Encrypt(data, dataSize, key, 6, 12, out, outSize);
}

void AES_192_Decrypt(unsigned char* data, size_t dataSize, unsigned char* key, size_t keySize, unsigned char* out, size_t outSize)
{
    if (keySize != 24 || dataSize > outSize)
        return;

    AES_Decrypt(data, dataSize, key, 6, 12, out, outSize);
}


void AES_256_Encrypt(unsigned char* data, size_t dataSize, unsigned char* key, size_t keySize, unsigned char* out, size_t outSize)
{
    if (keySize != 32 || dataSize > outSize)
        return;

    AES_Encrypt(data, dataSize, key, 8, 14, out, outSize);
}

void AES_256_Decrypt(unsigned char* data, size_t dataSize, unsigned char* key, size_t keySize, unsigned char* out, size_t outSize)
{
    if (keySize != 32 || dataSize > outSize)
        return;

    AES_Decrypt(data, dataSize, key, 8, 14, out, outSize);
}
