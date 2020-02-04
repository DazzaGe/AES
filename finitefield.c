/*
	AES

	This file provides functions necessary to apply finite field
        operations on a byte (representing a finite field)
*/

#include "finitefield.h"




typedef unsigned char BYTE;

const char inverse_table[256] = {0, 1, 141, 246, 203, 82, 123, 209, 232, 79, 41, 192, 176, 225, 229, 199, 116, 180, 170, 75, 153, 43, 96, 95, 88, 63, 253, 204, 255, 64, 238, 178, 58, 110, 90, 241, 85, 77, 168, 201, 193, 10, 152, 21, 48, 68, 162, 194, 44, 69, 146, 108, 243, 57, 102, 66, 242, 53, 32, 111, 119, 187, 89, 25, 29, 254, 55, 103, 45, 49, 245, 105, 167, 100, 171, 19, 84, 37, 233, 9, 237, 92, 5, 202, 76, 36, 135, 191, 24, 62, 34, 240, 81, 236, 97, 23, 22, 94, 175, 211, 73, 166, 54, 67, 244, 71, 145, 223, 51, 147, 33, 59, 121, 183, 151, 133, 16, 181, 186, 60, 182, 112, 208, 6, 161, 250, 129, 130, 131, 126, 127, 128, 150, 115, 190, 86, 155, 158, 149, 217, 247, 2, 185, 164, 222, 106, 50, 109, 216, 138, 132, 114, 42, 20, 159, 136, 249, 220, 137, 154, 251, 124, 46, 195, 143, 184, 101, 72, 38, 200, 18, 74, 206, 231, 210, 98, 12, 224, 31, 239, 17, 117, 120, 113, 165, 142, 118, 61, 189, 188, 134, 87, 11, 40, 47, 163, 218, 212, 228, 15, 169, 39, 83, 4, 27, 252, 172, 230, 122, 7, 174, 99, 197, 219, 226, 234, 148, 139, 196, 213, 157, 248, 144, 107, 177, 13, 214, 235, 198, 14, 207, 173, 8, 78, 215, 227, 93, 80, 30, 179, 91, 35, 56, 52, 104, 70, 3, 140, 221, 156, 125, 160, 205, 26, 65, 28};



BYTE FField_Add(BYTE b1, BYTE b2)
{
    return b1 ^ b2;
}


BYTE FField_xtime(BYTE b1)
{
    BYTE overflow = b1 & 128;

    b1 = b1 << 1;

    if (overflow == 0)
        return b1;

    return b1 ^ 0x1b;
}

BYTE FField_Multiply(BYTE b1, BYTE b2)
{
    BYTE result = 0;
    BYTE xtime_results[8];

    // xtime_results is an array of  1*b1, 2*b1, 4*b1, 8*b1, etc
    xtime_results[0] = b1;
    for (unsigned int i = 1; i < 8; i++)
    {
        xtime_results[i] = FField_xtime(xtime_results[i - 1]);
    }

    // multiply b1 by each bit of b2 (the xtime_results) and xor together
    for (unsigned int i = 0; i < 8; i++)
    {
        if ((b2 & 1) == 1)
            result ^= xtime_results[i];

        b2 = b2 >> 1;
    }

    return result;
}


BYTE FField_Inverse(unsigned char b1)
{
   return inverse_table[b1]; 
}
