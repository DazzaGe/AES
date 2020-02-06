/*
        AES

        This file contains the core functions for the Rijndael algorithm
*/

#include "rijndael.h"
#include "finitefield.h"
#include "stdio.h"




const unsigned int Nb = 4;
const unsigned char affine_matrix_array[64] =  {1, 0, 0, 0, 1, 1, 1, 1, 
                                                1, 1, 0, 0, 0, 1, 1, 1,
                                                1, 1, 1, 0, 0, 0, 1, 1,
                                                1, 1, 1, 1, 0, 0, 0, 1,
                                                1, 1, 1, 1, 1, 0, 0, 0,
                                                0, 1, 1, 1, 1, 1, 0, 0,
                                                0, 0, 1, 1, 1, 1, 1, 0,
                                                0, 0, 0, 1, 1, 1, 1, 1};
const unsigned char affine_vector = 0x63;



unsigned char SimpleMultiply(unsigned char b1, unsigned char b2)
{
    return b1 * b2;
}

unsigned char SBox(unsigned char b)
{
    // this will be using the less efficient manual way
    // of implementing the SBox for fun, obviously using
    // a lookup array is much faster
    ByteMatrix* affine_matrix = NULL;
    ByteMatrix* bit_matrix = NULL;
    ByteMatrix* result_matrix = NULL;
    unsigned char result_byte = 0x00;

    b = FField_Inverse(b);

    affine_matrix = ByteMatrix_New(8, 8);
    for (unsigned int r = 0; r < 8; r++)
    {
        for (unsigned int c = 0; c < 8; c++)
        {
            ByteMatrix_SetIndex(affine_matrix, r, c, affine_matrix_array[(8 * r) + c]);
        }
    }

    bit_matrix = ByteMatrix_New(8, 1);
    for (unsigned int i = 0; i < 8; i++)
    {
        ByteMatrix_SetIndex(bit_matrix, i, 0, (b >> i) & 1);
    }

    result_matrix = ByteMatrix_Mul(affine_matrix, bit_matrix, FField_Add, SimpleMultiply);
    for (unsigned int i = 0; i < 8; i++)
    {
        result_byte += ByteMatrix_GetIndex(result_matrix, i, 0) << i;
    }
    
    ByteMatrix_Del(affine_matrix); ByteMatrix_Del(result_matrix); ByteMatrix_Del(bit_matrix);

    return result_byte ^ affine_vector;
}


void SubBytes(ByteMatrix* state_array)
{
    unsigned char newByte = 0x00;

    for (unsigned int c = 0; c < Nb; c++)
    {
        for (unsigned int r = 0; r < 4; r++)
        {
            newByte = SBox(ByteMatrix_GetIndex(state_array, r, c));
            ByteMatrix_SetIndex(state_array, r, c, newByte);
        }
    }
}

void ShiftRows(ByteMatrix* state_array)
{
}

void MixColumns(ByteMatrix* state_array)
{
}

void AddRoundKey(ByteMatrix* state_array)
{
}
