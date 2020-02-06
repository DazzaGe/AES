/*
        AES
*/

#include "vector.h"




typedef struct ByteMatrix ByteMatrix; 

ByteMatrix*     ByteMatrix_New(unsigned int rows, unsigned int columns);
void            ByteMatrix_Del(ByteMatrix* mat);

unsigned int    ByteMatrix_GetRows(ByteMatrix* mat);
unsigned int    ByteMatrix_GetColumns(ByteMatrix* mat);

void            ByteMatrix_SetIndex(ByteMatrix* mat, unsigned int row, unsigned int column, unsigned char value);
unsigned char   ByteMatrix_GetIndex(ByteMatrix* mat, unsigned int row, unsigned int column);

ByteVector*     ByteMatrix_GetRow(ByteMatrix* mat, unsigned int row);
void            ByteMatrix_SetRow(ByteMatrix* mat, unsigned int row, ByteVector* vec);
ByteVector*     ByteMatrix_GetColumn(ByteMatrix* mat, unsigned int column);
void            ByteMatrix_SetColumn(ByteMatrix* mat, unsigned int column, ByteVector* vec);
