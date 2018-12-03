#include <stdint.h>

#include "drivers/instrument.h"
#include "drivers/k1.h"
#include "drivers/k2.h"
#include "drivers/k3.h"
#include "drivers/lcd.h"
#include "drivers/sn74hc166.h"
#include "drivers/sn74hc595.h"

#include "app.h"
#include "pattern.h"
#include "player.h"
#include "screen.h"
#include "tempo.h"
#include "volume.h"

static App_Mode CurrMode;
static App_Status CurrStatus;
static int CurrPattern;
static int CurrInstrument;
static int CurrStep;

static void OnPatternPanelPress(uint16_t toggle) {
  Pattern_ToggleCurrPattern(toggle);
  SN74HC595_SetState(Pattern_CurrPattern());
}

extern void App_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;
  
  LCD_INIT();
  K1_Init();
  K2_Init();
  K3_Init();
  SN74HC166_Init();
  SN74HC595_Init();

  Pattern_Init();
  Player_Init();
  Screen_Init();
  Tempo_Init();
  Volume_Init();

  App_SetCurrPattern(0);
  App_SetCurrInstrument(0);
  App_SetCurrMode(App_Mode_EditPattern);
  App_SetCurrStatus(App_Status_Stopped);
  App_SetCurrStep(0);
  
  K1_SetClickHandler(App_ToggleCurrMode);
  K2_SetClickHandler(App_ToggleCurrStatus);
  SN74HC166_SetRisingHandler(OnPatternPanelPress);
}

extern void App_SetCurrMode(App_Mode mode) {
  CurrMode = mode;
  Screen_UpdateCurrMode();
}

extern void App_ToggleCurrMode(void) {
  if (CurrMode == App_Mode_EditPattern)
    App_SetCurrMode(App_Mode_PlayPattern);
  else
    App_SetCurrMode(App_Mode_EditPattern);
}

extern App_Mode App_CurrMode(void) {
  return CurrMode;
}

extern void App_SetCurrStatus(App_Status status) {
  CurrStatus = status;
  Screen_UpdateCurrStatus();
}

extern void App_ToggleCurrStatus(void) {
  if (CurrStatus == App_Status_Stopped)
    App_SetCurrStatus(App_Status_Started);
  else
    App_SetCurrStatus(App_Status_Stopped);
}

extern App_Status App_CurrStatus(void) {
  return CurrStatus;
}

extern void App_SetCurrPattern(int pattern) {
  CurrPattern = pattern;
  Screen_UpdateCurrPattern();
}

extern int App_CurrPattern(void) {
  return CurrPattern;
}

extern void App_SetCurrInstrument(int instrument) {
  CurrInstrument = instrument;
  Screen_UpdateCurrInstrument();
}

extern int App_CurrInstrument(void) {
  return CurrInstrument;
}

extern void App_SetCurrStep(int step) {
  CurrStep = step;
  Screen_UpdateCurrStep();
}

extern void App_RotateCurrStep(void) {
  App_SetCurrStep((CurrStep >> 1) | (CurrStep << 15));
}

extern uint16_t App_CurrStep(void) {
  return CurrStep;
}
