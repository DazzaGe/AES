/*
        AES
*/

#include "matrix.h"



#define Nb 4




void    SubBytes(unsigned char state[4][Nb]);
void    ShiftRows(unsigned char state[4][Nb]);
void    MixColumns(unsigned char state[4][Nb]);
void    AddRoundKey(unsigned char state[4][Nb], unsigned char* key);
