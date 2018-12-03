#include "drivers/instrument.h"

#include "app.h"
#include "pattern.h"

uint16_t Data[Pattern_NumPatterns][Instrument_NumInstruments] = { 0 };

extern void Pattern_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;

  Instrument_Init();
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
