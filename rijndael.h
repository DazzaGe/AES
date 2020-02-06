/*
        AES
*/

#include "matrix.h"




void    SubBytes(ByteMatrix* state_array);
void    ShiftRows(ByteMatrix* state_array);
void    MixColumns(ByteMatrix* state_array);
void    AddRoundKey(ByteMatrix* state_array);
