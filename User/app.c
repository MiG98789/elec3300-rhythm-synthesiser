#include "drivers/cd4051b.h"
#include "drivers/instrument.h"
#include "drivers/k1.h"
#include "drivers/k2.h"
#include "drivers/k3.h"
#include "drivers/mastervolumepot.h"
#include "drivers/sn74hc166.h"
#include "drivers/sn74hc595.h"
#include "drivers/tempoencoder.h"
#include "drivers/timer.h"
#include "drivers/tsc2046.h"

#include "app.h"
#include "pattern.h"
#include "player.h"
#include "tempo.h"
#include "volume.h"

static int CurrPattern;
static int CurrInstrument;
static App_Mode CurrMode;

extern void App_Init(void) {
  CD4051B_Init();
  Instrument_Init();
  K1_Init();
  K2_Init();
  // K3_Init();
  MasterVolumePot_Init();
  SN74HC166_Init();
  SN74HC595_Init();
  
  Pattern_Init();
  Player_Init();
  Tempo_Init();
  Volume_Init();

  App_SetCurrPattern(0);
  App_SetCurrInstrument(0);
  App_SetCurrMode(App_Mode_PatternSelect);
}

extern void App_SetCurrMode(App_Mode mode) {
  CurrMode = mode;
}

extern App_Mode App_CurrMode(void) {
  return CurrMode;
}

extern void App_SetCurrPattern(int pattern) {
  CurrPattern = pattern;
}

extern int App_CurrPattern(void) {
  return CurrPattern;
}

extern void App_SetCurrInstrument(int instrument) {
  CurrInstrument = instrument;
}

extern int App_CurrInstrument(void) {
  return CurrInstrument;
}
