// File:                screen.h
// Module:              Screen
// Dependencies:        LCD, Poll, App
// Description:         Handles LCD rendering and touchscreen events

#ifndef __SCREEN_H
#define __SCREEN_H

void Screen_Init(void);

void Screen_UpdateCurrMode(void);
void Screen_UpdateCurrStatus(void);
void Screen_UpdateCurrPattern(void);
void Screen_UpdateCurrInstrument(void);
void Screen_UpdateCurrStep(void);

#endif
