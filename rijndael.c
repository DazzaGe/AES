/*
        AES

        This file contains the core functions for the Rijndael algorithm
*/

#include "rijndael.h"
#include "finitefield.h"
#include "stdio.h"




const unsigned char const affine_matrix_array[64] =  {1, 0, 0, 0, 1, 1, 1, 1, 
                                                1, 1, 0, 0, 0, 1, 1, 1,
                                                1, 1, 1, 0, 0, 0, 1, 1,
                                                1, 1, 1, 1, 0, 0, 0, 1,
                                                1, 1, 1, 1, 1, 0, 0, 0,
                                                0, 1, 1, 1, 1, 1, 0, 0,
                                                0, 0, 1, 1, 1, 1, 1, 0,
                                                0, 0, 0, 1, 1, 1, 1, 1};
const unsigned char const affine_vector = 0x63;
const unsigned char const mixcolumns_vector[4] = {0x02, 0x01, 0x01, 0x03};
const unsigned int shift_rows_amount[4] = {0, 1, 2, 3};



unsigned char SimpleMultiply(unsigned char b1, unsigned char b2)
{
    return b1 * b2;
}

unsigned char SBox(unsigned char byte)
{
    // this will be using the less efficient manual way
    // of implementing the SBox for fun, obviously using
    // a lookup array is much faster
    ByteMatrix* affine_matrix = NULL;
    ByteVector* bit_vector = NULL;
    ByteVector* result_vector = NULL;

    byte = FField_Inverse(byte);

    // setup affine matrix
    affine_matrix = ByteMatrix_New(8, 8);
    for (unsigned int r = 0; r < 8; r++)
    {
        for (unsigned int c = 0; c < 8; c++)
        {
            ByteMatrix_SetIndex(affine_matrix, r, c, affine_matrix_array[(8 * r) + c]);
        }
    }

    // setup inversebyte (b) as bit vector
    bit_vector = ByteVector_New(8);
    for (unsigned int bit = 0; bit < 8; bit++)
    {
        ByteVector_SetIndex(bit_vector, bit, (byte >> bit) & 1);
    }
    byte = 0x00;

    // perform affine transformation (using byte calculations in addition part rather than vector addition)
    result_vector = ByteMatrix_VectorMul(affine_matrix, bit_vector, 0, FField_Add, SimpleMultiply);
    for (unsigned int bit = 0; bit < 8; bit++)
    {
        byte += ByteVector_GetIndex(result_vector, bit) << bit;
    }
    byte ^= affine_vector;
    
    ByteMatrix_Del(affine_matrix); ByteVector_Del(result_vector); ByteVector_Del(bit_vector);

    return byte;
}

void SubBytes(unsigned char state[4][Nb])
{
    for (unsigned int r = 0; r < 4; r++)
    {
        for (unsigned int c = 0; c < Nb; c++)
        {
            state[r][c] = SBox(state[r][c]);
        }
    }
}


void ShiftRows(unsigned char state[4][Nb])
{
    unsigned char shifted[4][Nb];
    unsigned int newPos = 0;

    for (unsigned int r = 1; r < 4; r++)
    {
        for (unsigned int c = 0; c < Nb; c++)
        {
            newPos = (c - shift_rows_amount[r]) % Nb; 
            shifted[r][newPos] = state[r][c];
        }

        for (unsigned int c = 0; c < Nb; c++)
        {
            state[r][c] = shifted[r][c];
        }
    }
}


ByteMatrix* GetWordMatrix(ByteVector* word)
{
    ByteMatrix* wordMatrix = NULL;

    wordMatrix = ByteMatrix_New(4, 4);

    ByteMatrix_SetColumn(wordMatrix, 0, word);
    ByteVector_Shift(word, 1);
    ByteMatrix_SetColumn(wordMatrix, 1, word);
    ByteVector_Shift(word, 1);
    ByteMatrix_SetColumn(wordMatrix, 2, word);
    ByteVector_Shift(word, 1);
    ByteMatrix_SetColumn(wordMatrix, 3, word);
    ByteVector_Shift(word, 1);

    return wordMatrix;
}

void MixColumns(unsigned char state[4][Nb])
{
    ByteMatrix* multiplyMatrix = NULL;
    ByteVector* toMultiply = NULL;
    ByteVector* result = NULL;
    ByteVector* stateColumn = NULL;

    stateColumn = ByteVector_New(4);
    toMultiply = ByteVector_New(4);
    ByteVector_SetValues(toMultiply, mixcolumns_vector, 4);
    multiplyMatrix = GetWordMatrix(toMultiply);

    for (unsigned int c = 0; c < Nb; c++)
    {
        ByteVector_SetValues(stateColumn, (unsigned char*)state + (c * 4), 4);
        result = ByteMatrix_VectorMul(multiplyMatrix, stateColumn, 0, FField_Add, FField_Multiply);
        ByteVector_GetValues(result, (unsigned char*)state + (c * 4), 4);

        ByteVector_Del(result);
    }

    ByteMatrix_Del(multiplyMatrix); ByteVector_Del(toMultiply); ByteVector_Del(stateColumn);
}


void AddRoundKey(unsigned char state[4][Nb], unsigned char* key)
{
    for (unsigned int c = 0; c < Nb; c++)
    {
        for (unsigned int r = 0; r < 4; r++)
        {
            state[r][c] ^= key[(c * 4) + r];
        }
    }
}

/*
void Rijndael_Cipher(unsigned char* state, unsigned char* keything, unsigned int rounds)
{
    AddRoundKey(state, , 0);


    for (unsigned int r = 1; r < rounds; r++)
    {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, , r);
    }

    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state);
}
*/
