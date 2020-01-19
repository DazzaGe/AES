/*
	AES
*/




typedef struct ByteVector ByteVector;


ByteVector*	 	ByteVector_New(unsigned int length);
void			ByteVector_Del(ByteVector* vec);

void			ByteVector_Set_Index(ByteVector* vec, unsigned int pos, unsigned char value);
void			ByteVector_Set_Values(ByteVector* vec, unsigned char* values, unsigned int valuesLength);

unsigned char           ByteVector_Get_Index(ByteVector* vec, unsigned int pos);
