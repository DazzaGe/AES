/*
        AES

        This file contains the core functions for the Rijndael algorithm
*/

#include "rijndael.h"
#include "finitefield.h"
#include "matrix.h"




const uint8_t const affine_matrix_array[64] =  {1, 0, 0, 0, 1, 1, 1, 1, 
                                                1, 1, 0, 0, 0, 1, 1, 1,
                                                1, 1, 1, 0, 0, 0, 1, 1,
                                                1, 1, 1, 1, 0, 0, 0, 1,
                                                1, 1, 1, 1, 1, 0, 0, 0,
                                                0, 1, 1, 1, 1, 1, 0, 0,
                                                0, 0, 1, 1, 1, 1, 1, 0,
                                                0, 0, 0, 1, 1, 1, 1, 1};
const uint8_t affine_vector = 0x63;
const uint8_t const mixcolumns_vector[4] = {0x02, 0x01, 0x01, 0x03};
const unsigned int shift_rows_amount[4] = {0, 1, 2, 3};
const uint32_t const Rcon[14] = {69, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8};



uint8_t SimpleMultiply(uint8_t b1, uint8_t b2)
{
    return b1 * b2;
}

uint8_t SBox(uint8_t byte)
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

void SubBytes(uint8_t state[Nb][4])
{
    for (unsigned int c = 0; c < Nb; c++)
    {
        for (unsigned int r = 0; r < 4; r++)
        {
            state[c][r] = SBox(state[c][r]);
        }
    }
}


void ShiftRows(uint8_t state[Nb][4])
{
    uint8_t shifted[Nb][4];
    unsigned int newColumn = 0;

    for (unsigned int r = 1; r < 4; r++)
    {
        for (unsigned int c = 0; c < Nb; c++)
        {
            newColumn = (c - shift_rows_amount[r]) % Nb; 
            shifted[newColumn][r] = state[c][r];
        }

        for (unsigned int c = 0; c < Nb; c++)
        {
            state[c][r] = shifted[c][r];
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

void MixColumns(uint8_t state[Nb][4])
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
        ByteVector_SetValues(stateColumn, (uint8_t*)state + (c * 4), 4);
        result = ByteMatrix_VectorMul(multiplyMatrix, stateColumn, 0, FField_Add, FField_Multiply);
        ByteVector_GetValues(result, (uint8_t*)state + (c * 4), 4);

        ByteVector_Del(result);
    }

    ByteMatrix_Del(multiplyMatrix); ByteVector_Del(toMultiply); ByteVector_Del(stateColumn);
}


void AddRoundKey(uint8_t state[Nb][4], uint32_t* key)
{
    for (unsigned int i = 0; i < Nb; i++)
    {
        ((uint32_t*)state)[i] ^= ((uint32_t*)key)[i];
    }
}


void Rijndael_Cipher(uint8_t state[Nb][4], uint32_t* keySchedule, unsigned int Nr)
{
    AddRoundKey(state, keySchedule);

    for (unsigned int round = 1; round < Nr; round++)
    {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, keySchedule + (round * Nb));
    }

    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, keySchedule + (Nr * Nb));
}


uint32_t SubWord(uint32_t word)
{
    uint32_t sub;

    for (unsigned int i = 0; i < 4; i++)
    {
        sub = sub >> 8;
        sub += SBox((uint8_t)word) << 24;
        word = word >> 8;
    }

    return sub;
}

uint32_t RotWord(uint32_t word)
{
    // compiler must be little endian
    uint32_t rot = word >> 8;
    rot += word << 24;

    return rot;
}

void Rijndael_KeyExpansion(uint32_t* key, uint32_t* keySchedule, unsigned int Nk, unsigned int Nr)
{
    uint32_t temp;

    for (unsigned int i = 0; i < Nk; i++)
    {
        keySchedule[i] = key[i]; 
    }

    for (unsigned int i = Nk; i < (Nb * (Nr + 1)); i++)
    {
        temp = keySchedule[i - 1];

        if (i % Nk == 0)
            temp = SubWord(RotWord(temp)) ^ Rcon[i / Nk]; 
        else if (Nk > 6 && i % Nk == 4)
            temp = SubWord(temp);

        keySchedule[i] = keySchedule[i - Nk] ^ temp;
    }
}
