#include <stdint.h>

#include "drivers/instrument.h"
#include "drivers/k1.h"
#include "drivers/k2.h"
#include "drivers/k3.h"
#include "drivers/lcd.h"
#include "drivers/sn74hc166.h"
#include "drivers/sn74hc595.h"
#include "drivers/tempoencoder.h"
#include "drivers/tsc2046.h"

#include "app.h"
#include "pattern.h"
#include "player.h"
#include "screen.h"
#include "volume.h"

static App_Mode CurrMode = App_Mode_Play;
static App_Status CurrStatus = App_Status_Started;
static int CurrPattern = 0;
static int CurrInstrument = 0;
static uint16_t CurrStep = 0x1;

static int MsbIndex(uint16_t x) {
  int index = 15;
  while (x >>= 1)
    --index;
  return index;
}

static void OnPatternPanelPress(uint16_t toggle) {
  switch (CurrMode) {
    case App_Mode_Edit:
      switch (CurrStatus) {
        case App_Status_Started:
          Pattern_ToggleCurrPattern(toggle);
          SN74HC595_SetState(Pattern_CurrPattern());
          break;
        case App_Status_Stopped:
          App_SetCurrPattern(MsbIndex(toggle));
          break;
      }
      break;
    case App_Mode_Play:
      switch (CurrStatus) {
        case App_Status_Started:
          break;
        case App_Status_Stopped:
          break;
      }
      break;
  }
  if (CurrMode == App_Mode_Edit) {
    
  }
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
  TempoEncoder_Init();
  TSC2046_Init();

  Pattern_Init();
  Player_Init();
  Screen_Init();
  Volume_Init();

  App_ToggleCurrMode();
  App_ToggleCurrStatus();
  App_SetCurrPattern(0);
  App_SetCurrInstrument(0);
  
  K1_SetClickHandler(App_ToggleCurrMode);
  K2_SetClickHandler(App_ToggleCurrStatus);
  K3_SetClickHandler(App_ClearCurrPattern);
  SN74HC166_SetRisingHandler(OnPatternPanelPress);
}

extern void App_ToggleCurrMode(void) {
  SN74HC595_Clear();
  switch (CurrMode) {
    case App_Mode_Edit:
      CurrMode = App_Mode_Play;
      break;
    case App_Mode_Play:
      CurrMode = App_Mode_Edit;
      break;
  }
  Screen_UpdateCurrMode();
}

extern App_Mode App_CurrMode(void) {
  return CurrMode;
}

extern void App_ToggleCurrStatus(void) {
  SN74HC595_Clear();
  switch (CurrStatus) {
    case App_Status_Stopped:
      CurrStatus = App_Status_Started;
      SN74HC595_SetState(Pattern_CurrPattern());
      CurrStep = 0x1;
      Player_Start();
      break;
    case App_Status_Started:
      CurrStatus = App_Status_Stopped;
      SN74HC595_SetFlash(0x8000 >> CurrPattern);
      Player_Stop();
      break;
  }
  Screen_UpdateCurrStatus();
}

extern App_Status App_CurrStatus(void) {
  return CurrStatus;
}

extern void App_ClearCurrPattern(void) {
  if (CurrMode == App_Mode_Edit && CurrStatus == App_Status_Started) {
    Pattern_ClearCurrPattern();
    Screen_UpdateCurrPattern();
    SN74HC595_SetState(0);
  }
}

extern void App_SetCurrPattern(int pattern) {
  CurrPattern = pattern;
  Screen_UpdateCurrPattern();
  SN74HC595_SetFlash(0x8000 >> pattern);
}

extern int App_CurrPattern(void) {
  return CurrPattern;
}

extern void App_SetCurrInstrument(int instrument) {
  CurrInstrument = instrument;
  Screen_UpdateCurrInstrument();
  if (CurrMode == App_Mode_Edit && CurrStatus == App_Status_Started)
    SN74HC595_SetState(Pattern_CurrPattern());
}

extern int App_CurrInstrument(void) {
  return CurrInstrument;
}

extern void App_RenderStep(void) {
}

extern void App_RotateCurrStep(void) {
  CurrStep = App_NextStep();
  Screen_UpdateCurrStep();
  if (CurrStatus == App_Status_Started)
    SN74HC595_SetBlink(CurrStep);
}

extern uint16_t App_CurrStep(void) {
  return CurrStep;
}

extern uint16_t App_NextStep(void) {
  return (uint16_t) ((CurrStep >> 1) | (CurrStep << 15));
}
