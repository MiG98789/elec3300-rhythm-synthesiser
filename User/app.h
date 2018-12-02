// File:          app.h
// Module:        App
// Description:   Stores application state and coordinates communication between
//                modules.

#ifndef __APP_H
#define __APP_H

typedef enum {
  App_Mode_PatternSelect,
  App_Mode_PatternEdit,
  App_Mode_PatternPlay
} App_Mode;

void App_Init(void);

void App_SetCurrMode(App_Mode mode);
int App_CurrMode(void);

void App_SetCurrPattern(int pattern);
int App_CurrPattern(void);

void App_SetCurrInstrument(int instrument);
int App_CurrInstrument(void);

#endif
