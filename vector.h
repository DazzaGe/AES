/*
	AES
*/

#ifndef VECTOR_H
#define VECTOR_H

#include "stddef.h"
#include "stdint.h"




typedef struct ByteVector ByteVector;


ByteVector*	 	ByteVector_New(unsigned int length);
void			ByteVector_Del(ByteVector* vec);

unsigned int            ByteVector_GetLength(ByteVector* vec); 

void			ByteVector_SetIndex(ByteVector* vec, unsigned int pos, uint8_t value);
uint8_t           ByteVector_GetIndex(ByteVector* vec, unsigned int pos);

void			ByteVector_SetValues(ByteVector* vec, const uint8_t* values, size_t valuesLength);      // copy bytes from bytearray to vector
void                    ByteVector_GetValues(ByteVector* vec, uint8_t* toWrite, size_t amount);

void                    ByteVector_Copy(ByteVector* vec, ByteVector* copyto);   // copy values from first parameter into second

void                    ByteVector_Add(ByteVector* vec, ByteVector* other,
                        uint8_t (*AddFunction)(uint8_t val1, uint8_t val2));       // add both input vectors and store result in first input vector
uint8_t           ByteVector_Dot(ByteVector* vec, ByteVector* other, 
                        uint8_t (*AddFunction)(uint8_t val1, uint8_t val2), 
                        uint8_t (*MultiplyFunction)(uint8_t val1, uint8_t val2));  // perform dot product with both input vectors and return result

void                    ByteVector_Shift(ByteVector* vec, unsigned int amount);     // cyclically shifts the vectors values in the +index direction




#endif
