#ifndef __INSTRUMENT_H
#define __INSTRUMENT_H

#include <stdint.h>

#define Instrument_Count 8
#define Instrument_NoData

void Instrument_Init(void);
const int16_t* Instrument_Data(int i);
int16_t Instrument_Size(int i);
const char* Instrument_Name(int i);

#endif
