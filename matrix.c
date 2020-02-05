/*
        AES

        This file provides functions for the (byte) Matrix class,
        the matrix serves as an array of byte vectors.
*/

#include "matrix.h"
#include "vector.h"
#include "stdlib.h"




typedef struct ByteMatrix
{
    unsigned int    rows;
    unsigned int    columns;
    
    unsigned char   data[];
} ByteMatrix;




ByteMatrix* ByteMatrix_New(unsigned int rows, unsigned int columns)
{
    ByteMatrix* newMatrix = NULL; 
    size_t size = 0;

    if (rows < 1 || columns < 1) return NULL;

    size = sizeof(ByteMatrix) + (rows * columns) - 1;

    newMatrix = (ByteMatrix*)malloc(size);
    if (newMatrix == NULL) return NULL;
    
    newMatrix->rows = rows;
    newMatrix->columns = columns;
}

void ByteMatrix_Del(ByteMatrix* mat)
{
    free(mat);
}


unsigned int ByteMatrix_GetRows(ByteMatrix* mat)
{
    return mat->rows;
}

unsigned int ByteMatrix_GetColumns(ByteMatrix* mat)
{
    return mat->columns;
}

void ByteMatrix_SetIndex(ByteMatrix* mat, unsigned int row, unsigned int column, unsigned char value)
{
    unsigned int index = 0;

    if (row >= mat->rows || column >= mat->columns) return;

    index = (row * mat->columns) + column;
    mat->data[index] = value;
}

unsigned char ByteMatrix_GetIndex(ByteMatrix* mat, unsigned int row, unsigned int column)
{
    unsigned int index = 0;

    if (row >= mat->rows || column >= mat->columns) return 0;

    index = (row * mat->columns) + column;
    return mat->data[index];
}
