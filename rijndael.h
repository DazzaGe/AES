/*
        AES
*/

#include "matrix.h"



#define Nb 4




void    Rijndael_Cipher(unsigned char state[Nb][4], unsigned char* keySchedule, unsigned int Nr);
void    Rijndael_InvCipher();
void    Rijndael_KeyExpansion(unsigned char* key, unsigned char* keySchedule, unsigned int Nk, unsigned int Nr);

void    SubBytes(unsigned char state[Nb][4]);
void    ShiftRows(unsigned char state[Nb][4]);
void    MixColumns(unsigned char state[Nb][4]);
void    AddRoundKey(unsigned char state[Nb][4], unsigned char* key);

