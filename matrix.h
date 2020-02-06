/*
        AES
*/

#ifndef MATRIX_H
#define MATRIX_H

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

void            ByteMatrix_SwapRows(ByteMatrix* mat, unsigned int row1, unsigned int row2);
void            ByteMatrix_SwapColumns(ByteMatrix* mat, unsigned int column1, unsigned int column2);

void            ByteMatrix_ShiftRow(ByteMatrix* mat, unsigned int row, unsigned int amount);
void            ByteMatrix_ShiftColumn(ByteMatrix* mat, unsigned int column, unsigned int amount);

void            ByteMatrix_Add(ByteMatrix* mat, ByteMatrix* other, unsigned char (*AddFunction)(unsigned char val1, unsigned char val2));

ByteMatrix*     ByteMatrix_Mul(ByteMatrix* mat, ByteMatrix* other,
                unsigned char (*AddFunction)(unsigned char val1, unsigned char val2), \
                unsigned char (*MultiplyFunction)(unsigned char val1, unsigned char val2));
ByteVector*     ByteMatrix_VectorMul(ByteMatrix* mat, ByteVector* vec, unsigned char lhs,
                unsigned char (*AddFunction)(unsigned char val1, unsigned char val2), \
                unsigned char (*MultiplyFunction)(unsigned char val1, unsigned char val2));




#endif
