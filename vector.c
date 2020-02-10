/*
	AES

	This file provides useful vector functions for the (byte) vector class
*/

#include "vector.h"
#include "stdlib.h"
#include "string.h"




typedef struct ByteVector
{
    unsigned int    length;
    uint8_t	    data[];	
} ByteVector;



ByteVector* ByteVector_New(unsigned int length)
{
    ByteVector* newVector;
    size_t size;

    if (length < 1) return NULL;

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


unsigned int ByteVector_GetLength(ByteVector* vec) 
{
    return vec->length;
}


void ByteVector_SetIndex(ByteVector* vec, unsigned int pos, uint8_t value)
{
    if (pos < vec->length)
        (vec->data)[pos] = value;
}

uint8_t ByteVector_GetIndex(ByteVector* vec, unsigned int pos)
{
    if (pos < vec->length)
        return (vec->data)[pos];
    else
        return 0;
}


void ByteVector_SetValues(ByteVector* vec, const uint8_t* values, size_t valuesLength)
{
    if (valuesLength <= vec->length)
	memcpy(&(vec->data), values, valuesLength);
}

void ByteVector_GetValues(ByteVector* vec, uint8_t* toWrite, size_t amount)
{
    if (amount <= vec->length)
        memcpy(toWrite, &(vec->data), amount);
}


void ByteVector_Copy(ByteVector* vec, ByteVector* copyto)
{
    if (vec->length == copyto->length)
        memcpy(&(copyto->data), &(vec->data), vec->length);
}


void ByteVector_Add(ByteVector* vec, ByteVector* other, uint8_t (*AddFunction)(uint8_t val1, uint8_t val2))
{
    if (vec->length != other->length) 
        return;

    for (unsigned int i = 0; i < vec->length; i++)
    {
        vec->data[i] = AddFunction(vec->data[i], other->data[i]);
    }
}

uint8_t ByteVector_Dot(ByteVector* vec, ByteVector* other, uint8_t (*AddFunction)(uint8_t val1, uint8_t val2), uint8_t (*MultiplyFunction)(uint8_t val1, uint8_t val2))
{
    uint8_t sum = 0;
    uint8_t product = 0;

    if (vec->length != other->length)
        return 0;

    // multiplication via dot product
    for (unsigned int i = 0; i < vec->length; i++)
    {
        product = MultiplyFunction(vec->data[i], other->data[i]);
        sum = AddFunction(sum, product); 
    }

    return sum;
}


void ByteVector_Shift(ByteVector* vec, unsigned int amount)
{
    ByteVector* tempVector = NULL;
    unsigned int newPos = 0;

    tempVector = ByteVector_New(vec->length);
    ByteVector_Copy(vec, tempVector);

    for (unsigned int i = 0; i < vec->length; i++)
    {
        newPos = (i + amount) % vec->length;
        vec->data[newPos] = tempVector->data[i]; 
    }

    free(tempVector);
}
