#include "drivers/instrument.h"

#include "app.h"
#include "pattern.h"

uint16_t Data[Pattern_NumPatterns][Instrument_NumInstruments] = {
  { 0 },
  { 0 },
  { 0 },
  { 0 },
  { 0 },
  { 0 },
  { 0 },
  { 0 },
  { 0 },
  { 0 },
  { 0 },
  { 0 },
  { 0x8282, 0, 0x2082, 0x8208 , 0, 0, 0, 0xAAAA },
  { 0xC362, 0x100C, 0x0001, 0, 0, 0, 0x4000, 0x820A },
  { 0x8888, 0x1228, 0, 0, 0, 0, 0, 0xAAAA },
  { 0x8888, 0x500A, 0x0600, 0, 0, 0, 0, 0x8888 }
};

extern void Pattern_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;

  Instrument_Init();
}

extern void Pattern_ClearCurrPattern(void) {
  const int pattern = App_CurrPattern();
  const int instrument = App_CurrInstrument();
  Data[pattern][instrument] = 0;
}

extern void Pattern_ToggleCurrPattern(uint16_t toggle) {
  const int pattern = App_CurrPattern();
  const int instrument = App_CurrInstrument();
  Data[pattern][instrument] ^= toggle;
}

extern uint16_t Pattern_CurrPattern(void) {
  const int pattern = App_CurrPattern();
  const int instrument = App_CurrInstrument();
  return Data[pattern][instrument];
}

extern const uint16_t* Pattern_Data(int pattern) {
  return Data[pattern];
}
