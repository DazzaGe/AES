/*
        AES
*/

#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "aes.h"

#define AES_128_BLOCK_LEN   16
#define AES_192_BLOCK_LEN   16
#define AES_256_BLOCK_LEN   16  




typedef enum
{
    AES_128,
    AES_192,
    AES_256,
} cipher;

typedef enum
{
    ISO_10126,    
} padding;



void AddPadding(char* buffer, size_t size, size_t amount, padding method)
{
    srand(time(NULL));

    switch (method)
    {
        case ISO_10126:
            for (unsigned int i = amount; i < size; i++)
            {
                buffer[i] = rand() % 256;
            }

            break;
    }
}


unsigned char* Encrypt(const char* data, size_t dataSize, const char* key, size_t keySize, cipher algo)
{
    unsigned char* out = NULL;
    size_t outSize = 0;
    char* excessBuffer;    // ecrypting excess with padding saves having to copy the entire data buffer and adding padding to that
    size_t excessAmount = 0;

    switch (algo)
    {
        case AES_128:
            excessAmount = dataSize % AES_128_BLOCK_LEN;
            excessBuffer = malloc(AES_128_BLOCK_LEN);
            outSize = dataSize - excessAmount + AES_128_BLOCK_LEN;
            out = malloc(outSize);

            AES_128_Encrypt((char*)data, dataSize - excessAmount, (char*)key, keySize, out, dataSize); 
            if (excessAmount > 0)
            {
                AddPadding(excessBuffer, AES_128_BLOCK_LEN, excessAmount, ISO_10126);
                AES_128_Encrypt(excessBuffer, AES_128_BLOCK_LEN, (char*)key, keySize, out + dataSize - excessAmount, AES_128_BLOCK_LEN);
            }

            break;

        case AES_192:
            excessAmount = dataSize % AES_192_BLOCK_LEN;
            break;

        case AES_256:
            excessAmount = dataSize % AES_256_BLOCK_LEN;
            break;
    }

    free(excessBuffer);

    return out;
}

void Decrypt()
{
}

int main(int argc, const char* argv[])
{
    return 0;
}

