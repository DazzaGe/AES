/*
        AES

        This file provides functions for the (byte) Matrix class,
        the matrix serves as an array of byte vectors.
*/

#include "matrix.h"
#include "stdlib.h"
#include "stdio.h"




typedef struct ByteMatrix
{
    unsigned int    rows;
    unsigned int    columns;
    
    unsigned char*  data[];
} ByteMatrix;




ByteMatrix* ByteMatrix_New(unsigned int rows, unsigned int columns)
{
    ByteMatrix* newMatrix = NULL; 
    size_t pointerBlockSize = 0;
    size_t size = 0;

    if (rows < 1 || columns < 1) return NULL;

    pointerBlockSize = rows * sizeof(unsigned char*);
    size = sizeof(ByteMatrix) + pointerBlockSize + (rows * columns);

    newMatrix = (ByteMatrix*)malloc(size);
    if (newMatrix == NULL) return NULL;
    
    newMatrix->rows = rows;
    newMatrix->columns = columns;
    for (unsigned int i = 0; i < rows; i++)
    {
        newMatrix->data[i] = (unsigned char*)&newMatrix->data + pointerBlockSize + (i * columns);
    }

    return newMatrix;
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
    if (row >= mat->rows || column >= mat->columns) return;

    mat->data[row][column] = value;
}

unsigned char ByteMatrix_GetIndex(ByteMatrix* mat, unsigned int row, unsigned int column)
{
    if (row >= mat->rows || column >= mat->columns) return 0;

    return mat->data[row][column];
}


ByteVector* ByteMatrix_GetRow(ByteMatrix* mat, unsigned int row)
{
    ByteVector* rowVec = NULL;

    if (row >= mat->rows) return NULL;

    rowVec = ByteVector_New(mat->columns);
    ByteVector_SetValues(rowVec, mat->data[row], mat->columns);

    return rowVec;
}

void ByteMatrix_SetRow(ByteMatrix* mat, unsigned int row, ByteVector* vec)
{
    if (row >= mat->rows || vec == NULL || ByteVector_GetLength(vec) != mat->columns) return;

    for (unsigned int c = 0; c < mat->columns; c++)
    {
        mat->data[row][c] = ByteVector_GetIndex(vec, c);            
    }
}

ByteVector* ByteMatrix_GetColumn(ByteMatrix* mat, unsigned int column)
{
    ByteVector* columnVec = NULL;

    if (column >= mat->columns) return NULL;

    columnVec = ByteVector_New(mat->rows);
    for (unsigned int r = 0; r < mat->rows; r++)
    {
        ByteVector_SetIndex(columnVec, r, mat->data[r][column]);
    }

    return columnVec;
}

void ByteMatrix_SetColumn(ByteMatrix* mat, unsigned int column, ByteVector* vec)
{
    if (column >= mat->columns || vec == NULL || ByteVector_GetLength(vec) != mat->rows) return;

    for (unsigned int r = 0; r < mat->columns; r++)
    {
        mat->data[r][column] = ByteVector_GetIndex(vec, r);            
    }
}
