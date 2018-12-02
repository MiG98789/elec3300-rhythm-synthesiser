// File:          pattern.h
// Module:        Pattern
// Description:   Stores user-created pattern arrays. The j-th bit in the i-th
//                element of a pattern array is 1 if the i-th instrument is
//                active on the j-th step of the pattern, and 0 otherwise.

#ifndef __PATTERN_H
#define __PATTERN_H

#include <stdint.h>

#define Pattern_Count 16

void Pattern_Init(void);

const uint16_t* Pattern_Data(int i);

#endif
