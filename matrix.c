/*
        AES

        This file provides functions for the (byte) Matrix class,
        the matrix serves as an array of byte vectors.
*/

#include "matrix.h"
#include "stdlib.h"




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


void ByteMatrix_SwapRows(ByteMatrix* mat, unsigned int row1, unsigned int row2)
{
    unsigned char* temp = NULL;

    if (row1 >= mat->rows || row2 >= mat->rows) return;

    temp = mat->data[row1];
    mat->data[row1] = mat->data[row2];
    mat->data[row2] = temp;
}

void ByteMatrix_SwapColumns(ByteMatrix* mat, unsigned int column1, unsigned int column2)
{
    unsigned char temp = 0x00;

    if (column1 >= mat->columns || column2 >= mat->columns) return;

    for (unsigned int r = 0; r < mat->rows; r++)
    {
        temp = mat->data[r][column1];
        mat->data[r][column1] = mat->data[r][column2];
        mat->data[r][column2] = temp;
    }
}


void ByteMatrix_ShiftRow(ByteMatrix* mat, unsigned int row, unsigned int amount)
{
    ByteVector* tempRow = NULL;

    if (row >= mat->rows) return;   

    // very simple solution, but not very efficient, efficiency isn't needed as AES matrix's aren't big
    tempRow = ByteMatrix_GetRow(mat, row);
    ByteVector_Shift(tempRow, amount);

    ByteMatrix_SetRow(mat, row, tempRow);
    ByteVector_Del(tempRow);
}

void ByteMatrix_ShiftColumn(ByteMatrix* mat, unsigned int column, unsigned int amount)
{
    ByteVector* tempColumn = NULL;

    if (column >= mat->columns) return;


    // very simple solution, but not very efficient, efficiency isn't needed as AES matrix's aren't big
    tempColumn = ByteMatrix_GetColumn(mat, column);
    ByteVector_Shift(tempColumn, amount);

    ByteMatrix_SetColumn(mat, column, tempColumn);
    ByteVector_Del(tempColumn);
}


void ByteMatrix_Add(ByteMatrix* mat, ByteMatrix* other, unsigned char (*AddFunction)(unsigned char val1, unsigned char val2))
{
    if (mat->rows != other->rows || mat->columns != other->columns) return;

    for (unsigned int r = 0; r < mat->rows; r++)
    {
        for (unsigned int c = 0; c < mat->columns; c++)
        {
            mat->data[r][c] = AddFunction(mat->data[r][c], other->data[r][c]);
        }
    }
}


ByteMatrix* ByteMatrix_Mul(ByteMatrix* mat, ByteMatrix* other,
                unsigned char (*AddFunction)(unsigned char val1, unsigned char val2), \
                unsigned char (*MultiplyFunction)(unsigned char val1, unsigned char val2))
{
    ByteMatrix* resultMat = NULL;
    ByteVector* rowVec = NULL;
    ByteVector* colVec = NULL;
    unsigned char dotResult = 0x00;

    if (mat->columns != other->rows) return NULL;

    resultMat = ByteMatrix_New(mat->rows, other->columns); 
    
    for (unsigned int r = 0; r < mat->rows; r++)
    {
        rowVec = ByteMatrix_GetRow(mat, r);

        for (unsigned int c = 0; c < other->columns; c++)
        {
            colVec = ByteMatrix_GetColumn(other, c);

            dotResult = ByteVector_Dot(rowVec, colVec, AddFunction, MultiplyFunction);
            resultMat->data[r][c] = dotResult;
        }
    }

    return resultMat;
}
