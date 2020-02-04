/*
	AES

	This file provides useful vector functions for the (byte) vector class
*/

#include "vector.h"
#include "finitefield.h"
#include "stdlib.h"
#include "string.h"

#define BYTE unsigned char



typedef struct ByteVector
{
    unsigned int    length;
    BYTE	    data[];	
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


unsigned int ByteVector_GetLength(ByteVector* vec) 
{
    return vec->length;
}


void ByteVector_SetIndex(ByteVector* vec, unsigned int pos, BYTE value)
{
    if (pos < vec->length)
        (vec->data)[pos] = value;
}

void ByteVector_SetValues(ByteVector* vec, BYTE* values, size_t valuesLength)
{
    if (valuesLength <= vec->length)
	memcpy(&(vec->data), values, valuesLength);
}


BYTE ByteVector_GetIndex(ByteVector* vec, unsigned int pos)
{
    if (pos < vec->length)
        return (vec->data)[pos];
    else
        return 0;
}


void ByteVector_Copy(ByteVector* vec, ByteVector* copyto)
{
    if (vec->length == copyto->length)
        memcpy(&(copyto->data), &(vec->data), vec->length);
}


void ByteVector_Add(ByteVector* vec, ByteVector* other, BYTE (*Add_Function)(BYTE val1, BYTE val2))
{
    if (vec->length != other->length) 
        return;

    for (unsigned int i = 0; i < vec->length; i++)
    {
        vec->data[i] = Add_Function(vec->data[i], other->data[i]);
    }
}

BYTE ByteVector_Dot(ByteVector* vec, ByteVector* other, BYTE (*Add_Function)(BYTE val1, BYTE val2), BYTE (*Multiply_Function)(BYTE val1, BYTE val2))
{
    BYTE sum = 0;
    BYTE product = 0;

    if (vec->length != other->length)
        return 0;

    // multiplication via dot product
    for (unsigned int i = 0; i < vec->length; i++)
    {
        product = Multiply_Function(vec->data[i], other->data[i]);
        sum = Add_Function(sum, product); 
    }

    return sum;
}
