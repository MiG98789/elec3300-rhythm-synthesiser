// File:          app.h
// Module:        App
// Description:   Stores application state and coordinates communication between
//                modules.

#ifndef __APP_H
#define __APP_H

#include <stdint.h>

typedef enum {
  App_Mode_Edit,
  App_Mode_Play
} App_Mode;

typedef enum {
  App_Status_Stopped,
  App_Status_Started
} App_Status;

void App_Init(void);

// void App_SetCurrMode(App_Mode mode);
void App_ToggleCurrMode(void);
App_Mode App_CurrMode(void);

// void App_SetCurrStatus(App_Status status);
void App_ToggleCurrStatus(void);
App_Status App_CurrStatus(void);

void App_SetCurrPattern(int pattern);
int App_CurrPattern(void);

void App_SetCurrInstrument(int instrument);
int App_CurrInstrument(void);

void App_RotateCurrStep(void);
uint16_t App_CurrStep(void);

#endif
