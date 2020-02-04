/*
	AES
*/

#include "stddef.h"




typedef struct ByteVector ByteVector;


ByteVector*	 	ByteVector_New(unsigned int length);
void			ByteVector_Del(ByteVector* vec);

unsigned int            ByteVector_GetLength(ByteVector* vec); 

void			ByteVector_SetIndex(ByteVector* vec, unsigned int pos, unsigned char value);
void			ByteVector_SetValues(ByteVector* vec, unsigned char* values, size_t valuesLength);      // copy bytes from bytearray to vector

unsigned char           ByteVector_GetIndex(ByteVector* vec, unsigned int pos);

void                    ByteVector_Copy(ByteVector* vec, ByteVector* copyto);   // copy values from first parameter into second

void                    ByteVector_Add(ByteVector* vec, ByteVector* other,
                        unsigned char (*Add_Function)(unsigned char val1, unsigned char val2));       // add both input vectors and store result in first input vector
unsigned char           ByteVector_Dot(ByteVector* vec, ByteVector* other, 
                        unsigned char (*Add_Function)(unsigned char val1, unsigned char val2), unsigned char (*Multiply_Function)(unsigned char val1, unsigned char val2));  // perform dot product with both input vectors and return result

void                    ByteVector_Shift(ByteVector* vec, unsigned int amount);     // cyclically shifts the vectors values in the +index direction
