/*
        AES
*/

#ifndef RIJNDAEL_H
#define RIJNDAEL_H

#include <stdint.h>



#define Nb 4




void    Rijndael_Cipher(uint8_t state[Nb][4], uint32_t* keySchedule, unsigned int Nr);
void    Rijndael_InvCipher(uint8_t state[Nb][4], uint32_t* keySchedule, unsigned int Nr);
void    Rijndael_KeyExpansion(uint32_t* key, uint32_t* keySchedule, unsigned int Nk, unsigned int Nr);

#endif // RIJNDAEL_H
