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

void            ByteMatrix_SetIndex(ByteMatrix* mat, unsigned int row, unsigned int column, uint8_t value);
uint8_t   ByteMatrix_GetIndex(ByteMatrix* mat, unsigned int row, unsigned int column);

ByteVector*     ByteMatrix_GetRow(ByteMatrix* mat, unsigned int row);
void            ByteMatrix_SetRow(ByteMatrix* mat, unsigned int row, ByteVector* vec);
ByteVector*     ByteMatrix_GetColumn(ByteMatrix* mat, unsigned int column);
void            ByteMatrix_SetColumn(ByteMatrix* mat, unsigned int column, ByteVector* vec);

void            ByteMatrix_SwapRows(ByteMatrix* mat, unsigned int row1, unsigned int row2);
void            ByteMatrix_SwapColumns(ByteMatrix* mat, unsigned int column1, unsigned int column2);

void            ByteMatrix_ShiftRow(ByteMatrix* mat, unsigned int row, unsigned int amount);
void            ByteMatrix_ShiftColumn(ByteMatrix* mat, unsigned int column, unsigned int amount);

void            ByteMatrix_Add(ByteMatrix* mat, ByteMatrix* other, uint8_t (*AddFunction)(uint8_t val1, uint8_t val2));

ByteMatrix*     ByteMatrix_Mul(ByteMatrix* mat, ByteMatrix* other,
                uint8_t (*AddFunction)(uint8_t val1, uint8_t val2), \
                uint8_t (*MultiplyFunction)(uint8_t val1, uint8_t val2));
ByteVector*     ByteMatrix_VectorMul(ByteMatrix* mat, ByteVector* vec, uint8_t lhs,
                uint8_t (*AddFunction)(uint8_t val1, uint8_t val2), \
                uint8_t (*MultiplyFunction)(uint8_t val1, uint8_t val2));




#endif
