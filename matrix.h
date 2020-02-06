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

void            ByteMatrix_SwapRows(ByteMatrix* mat, unsigned int row1, unsigned int row2);
void            ByteMatrix_SwapColumns(ByteMatrix* mat, unsigned int column1, unsigned int column2);

void            ByteMatrix_ShiftRow(ByteMatrix* mat, unsigned int row, unsigned int amount);
void            ByteMatrix_ShiftColumn(ByteMatrix* mat, unsigned int column, unsigned int amount);

ByteMatrix*     ByteMatrix_Multiplication(ByteMatrix* mat, ByteMatrix* other,
                unsigned char (*Add_Function)(unsigned char val1, unsigned char val2), \
                unsigned char (*Multiply_Function)(unsigned char val1, unsigned char val2));
