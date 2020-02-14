/*
        AES
*/

#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
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


typedef struct 
{
    unsigned char*  data;
    size_t          dataLength;
    unsigned char*  key;
    size_t          keyLength;

    unsigned int    encrypt     : 1;
    cipher          _cipher     : 4;
    padding         _padding    : 4;
} Options;




void AddPadding(unsigned char* buffer, size_t size, size_t amount, padding method)
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


size_t Encrypt(unsigned char* data, size_t dataSize, const char* key, size_t keySize, unsigned char** p_out, cipher algo, padding pad)
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
            outSize = excessAmount ? dataSize - excessAmount + AES_128_BLOCK_LEN : dataSize;
            out = malloc(outSize);

            AES_128_Encrypt((char*)data, dataSize - excessAmount, (char*)key, keySize, out, dataSize); 
            if (excessAmount > 0)
            {
                AddPadding(excessBuffer, AES_128_BLOCK_LEN, excessAmount, pad);
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

    *p_out = out;
    return outSize;
}

void Decrypt()
{
}


int main(int argc, const char* argv[])
{
    unsigned char* out;
    size_t outSize;
    Options options;

    if (argc < 4)
        return 1;

    options.data = (unsigned char*)argv[1];
    options.dataLength = strlen(argv[1]);
    options.key = NULL;
    options.encrypt = 1;
    options._cipher = AES_128;
    options._padding = ISO_10126; 

    for (int i = 2; i < argc; i++)
    {
        if (argv[i][0] != '-')
            return 1;

        switch(argv[i][1])
        {
            case 'k':
                if (i == argc)
                    return 1;

                options.key = (unsigned char*)argv[i + 1];
                options.keyLength = strlen(options.key);
                i++;
                break;

            case 'e':
                options.encrypt = 1;
                break;

            case 'd':
                options.encrypt = 0;
                break;

            case 'c':
                if (i == argc)
                    return 1;

                options._cipher = (int)((argv[i + 1])[0]);
                i++;
                break;

            case 'p':
                if (i == argc)
                    return 1;

                options._padding = (int)((argv[i + 1])[0]);
                i++;
                break;
        }
    }

    if (options.key == NULL)
        return 1;

    if (options.encrypt)
        outSize = Encrypt(options.data, options.dataLength, options.key, options.keyLength, &out, options._cipher, options._padding);
    //else
        //outSize = Decrypt();

    printf("Result Bytes: ");
    for (unsigned int i = 0; i < outSize; i++)
    {
        printf("%02X ", out[i]);
    }
    printf("\n");

    return 0;
}

