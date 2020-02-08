/*
	AES

	This file provides useful vector functions for the (byte) vector class
*/

#include "vector.h"
#include "stdlib.h"
#include "string.h"




typedef unsigned char BYTE;

typedef struct ByteVector
{
    unsigned int    length;
    BYTE	    data[];	
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


void ByteVector_SetIndex(ByteVector* vec, unsigned int pos, BYTE value)
{
    if (pos < vec->length)
        (vec->data)[pos] = value;
}

BYTE ByteVector_GetIndex(ByteVector* vec, unsigned int pos)
{
    if (pos < vec->length)
        return (vec->data)[pos];
    else
        return 0;
}


void ByteVector_SetValues(ByteVector* vec, const BYTE* values, size_t valuesLength)
{
    if (valuesLength <= vec->length)
	memcpy(&(vec->data), values, valuesLength);
}

void ByteVector_GetValues(ByteVector* vec, BYTE* toWrite, size_t amount)
{
    if (amount <= vec->length)
        memcpy(toWrite, &(vec->data), amount);
}


void ByteVector_Copy(ByteVector* vec, ByteVector* copyto)
{
    if (vec->length == copyto->length)
        memcpy(&(copyto->data), &(vec->data), vec->length);
}


void ByteVector_Add(ByteVector* vec, ByteVector* other, BYTE (*AddFunction)(BYTE val1, BYTE val2))
{
    if (vec->length != other->length) 
        return;

    for (unsigned int i = 0; i < vec->length; i++)
    {
        vec->data[i] = AddFunction(vec->data[i], other->data[i]);
    }
}

BYTE ByteVector_Dot(ByteVector* vec, ByteVector* other, BYTE (*AddFunction)(BYTE val1, BYTE val2), BYTE (*MultiplyFunction)(BYTE val1, BYTE val2))
{
    BYTE sum = 0;
    BYTE product = 0;

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
