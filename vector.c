/*
	AES

	This file provides useful vector functions for the (byte) vector class
*/

#include "vector.h"
#include "stdlib.h"
#include "string.h"



typedef struct ByteVector
{
    unsigned int	length;
    unsigned char	data[];	
} ByteVector;



ByteVector* ByteVector_New(unsigned int length)
{
    ByteVector* newVector;
    size_t size;


    size = sizeof(ByteVector) + length;

    newVector = (ByteVector*)malloc(size);
    if (newVector == NULL) return NULL;

    memset(newVector, 0, size);
    newVector->length = length;

    return newVector;
}

void ByteVector_Del(ByteVector* vec)
{
    free(vec);	
}


void ByteVector_Set_Index(ByteVector* vec, unsigned int pos, unsigned char value)
{
    if (pos < vec->length)
        (vec->data)[pos] = value;
}

void ByteVector_Set_Values(ByteVector* vec, unsigned char* values, unsigned int valuesLength)
{
    if (valuesLength <= vec->length)
	memcpy(&(vec->data), values, valuesLength);
}


unsigned char ByteVector_Get_Index(ByteVector* vec, unsigned int pos)
{
    if (pos < vec->length)
        return (vec->data)[pos];
    else
        return 0;
}
