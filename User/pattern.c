#include "drivers/instrument.h"

#include "pattern.h"

uint16_t Data[Pattern_Count][Instrument_Count] = { 0 };

extern void Pattern_Init(void) {
}

extern const uint16_t* Pattern_Data(int i) {
  return Data[i];
}
