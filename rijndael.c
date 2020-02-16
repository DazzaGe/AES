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
const uint8_t const inv_mixcolumns_vector[4] = {0x0e, 0x09, 0x0d, 0x0b};
const unsigned int shift_rows_amount[4] = {0, 1, 2, 3};
const uint32_t const Rcon[14] = {69, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8};
const uint8_t InvSBox[256] =   {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, 
                                0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 
                                0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, 
                                0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, 
                                0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 
                                0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, 
                                0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, 
                                0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 
                                0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, 
                                0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, 
                                0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 
                                0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, 
                                0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, 
                                0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 
                                0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, 
                                0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};



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

void InvSubBytes(uint8_t state[Nb][4])
{
    for (unsigned int c = 0; c < Nb; c++)
    {
        for (unsigned int r = 0; r < 4; r++)
        {
            state[c][r] = InvSBox[state[c][r]];
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

void InvShiftRows(uint8_t state[Nb][4])
{
    uint8_t shifted[Nb][4];
    unsigned int newColumn = 0;

    for (unsigned int r = 1; r < 4; r++)
    {
        for (unsigned int c = 0; c < Nb; c++)
        {
            newColumn = (c + shift_rows_amount[r]) % Nb;
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

void InvMixColumns(uint8_t state[Nb][4])
{
    ByteMatrix* multiplyMatrix = NULL;
    ByteVector* toMultiply = NULL;
    ByteVector* result = NULL;
    ByteVector* stateColumn = NULL;

    stateColumn = ByteVector_New(4);
    toMultiply = ByteVector_New(4);
    ByteVector_SetValues(toMultiply, inv_mixcolumns_vector, 4);
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

void Rijndael_InvCipher(uint8_t state[Nb][4], uint32_t* keySchedule, unsigned int Nr)
{
    AddRoundKey(state, keySchedule + (Nr * Nb));

    for (unsigned int round = Nr - 1; round > 0; round--)
    {
       InvShiftRows(state);
       InvSubBytes(state);
       AddRoundKey(state, keySchedule + (round * Nb));
       InvMixColumns(state);
    }

    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, keySchedule);
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
