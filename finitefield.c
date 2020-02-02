/*
	AES

	This file provides functions necessary to apply finite field
        operations on a byte (representing a finite field)
*/

#include "finitefield.h"

#define BYTE unsigned char




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
