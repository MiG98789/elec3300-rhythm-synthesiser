#include <stdio.h>

#include "drivers/instrument.h"
#include "drivers/lcd.h"
#include "drivers/poll.h"
#include "drivers/tempoencoder.h"
#include "drivers/tsc2046.h"

#include "app.h"
#include "screen.h"
#include "volume.h"

static void DrawButtons(void) {
  static uint8_t instrumentIndex = 0;
  static uint16_t xPos = 0x00;
  static uint16_t yPos = 0x00;

  for (instrumentIndex = 0; instrumentIndex < 8; instrumentIndex++) {
    if (instrumentIndex < 4) {
      xPos = 0x00;
      yPos = 0x80 + 0x30*instrumentIndex;
    } else {
      xPos = 0x78;
      yPos = 0x80 + 0x30*(instrumentIndex - 4);
    }

    if (instrumentIndex == App_CurrInstrument())
      LCD_DrawButton(xPos, yPos, Instrument_Name(instrumentIndex), BLACK, CYAN);
    else
      LCD_DrawButton(xPos, yPos, Instrument_Name(instrumentIndex), BLACK, WHITE);
  }
}

static void OnTouch(uint8_t x, uint8_t y) {
  static uint8_t prevInstrument = 0;
  uint16_t yScale = 0;
  char output[9] = "";
  snprintf(output, sizeof(output), "(%d,%d)", x, y);

  LCD_DrawString(0x78, 0x60, "         ");
  LCD_DrawString(0x78, 0x60, output);

  yScale = 320 - ((320*y) >> 8);
  if (x > 0 && x <= 127) {
    if (yScale >= 0x80 && yScale < 0xa0)        App_SetCurrInstrument(0);
    else if (yScale >= 0xa0 && yScale < 0xd0)   App_SetCurrInstrument(1);
    else if (yScale >= 0xd0 && yScale < 0x100)  App_SetCurrInstrument(2);
    else if (yScale >= 0x100 && yScale < 0x130) App_SetCurrInstrument(3);
  } else if (x > 127 && x < 255) {
    if (yScale >= 0x80 && yScale < 0xa0)        App_SetCurrInstrument(4);
    else if (yScale >= 0xa0 && yScale < 0xd0)   App_SetCurrInstrument(5);
    else if (yScale >= 0xd0 && yScale < 0x100)  App_SetCurrInstrument(6);
    else if (yScale >= 0x100 && yScale < 0x130) App_SetCurrInstrument(7);
  }

  if (prevInstrument != App_CurrInstrument())
    DrawButtons();

  prevInstrument = App_CurrInstrument();
}

static void OnTempoChange(int tempoBPM) {
  char output[3] = "";
  if (tempoBPM < 100)
    snprintf(output, sizeof(output), "%d ", tempoBPM);
  else
    snprintf(output, sizeof(output), "%d", tempoBPM);
  
  LCD_DrawString(0x78, 0x50, output);
}

static void VolumePoll(void) {
  static uint16_t tempVolume = 0;
  static char tempBuffer[4];

  tempVolume = Volume_MasterVolume();

  if (tempVolume < 10)
    snprintf(tempBuffer, sizeof(tempBuffer), "%d   ", tempVolume);
  else if (tempVolume < 100)
    snprintf(tempBuffer, sizeof(tempBuffer), "%d  ", tempVolume);
  else if (tempVolume < 1000)
    snprintf(tempBuffer, sizeof(tempBuffer), "%d ", tempVolume);
  else
    snprintf(tempBuffer, sizeof(tempBuffer), "%d", tempVolume);

  LCD_DrawString(0x78, 0x70, tempBuffer);
}

extern void Screen_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;
  
  LCD_INIT();
  Poll_Init();
  TSC2046_Init();
  Volume_Init();

  LCD_DrawString(0x00, 0x00, "Mode:");
  LCD_DrawString(0x00, 0x10, "Status:");
  LCD_DrawString(0x00, 0x20, "Pattern:");
  LCD_DrawString(0x00, 0x30, "Instrument:");
  LCD_DrawString(0x00, 0x40, "Step:");
  LCD_DrawString(0x00, 0x50, "Tempo:");
  LCD_DrawString(0x00, 0x60, "Touch (X,Y):");
  LCD_DrawString(0x00, 0x70, "Master vol:");

  LCD_DrawString(0x78, 0x50, "120");
  LCD_DrawString(0x78, 0x60, "(0,0)");
  
  DrawButtons();

  Poll_AddHandler(VolumePoll);
  TempoEncoder_SetChangeHandler(OnTempoChange);
  TSC2046_SetTouchHandler(OnTouch);
  
}

extern void Screen_UpdateCurrMode(void) {
  switch (App_CurrMode()) {
    case App_Mode_Edit:
      LCD_DrawString(0x78, 0x00, "Edit");
      break;
    case App_Mode_Play:
      LCD_DrawString(0x78, 0x00, "Play");
      break;
  }
}

extern void Screen_UpdateCurrStatus(void) {
  if (App_CurrStatus() == App_Status_Started)
    LCD_DrawString(0x78, 0x10, "Started");
  else
    LCD_DrawString(0x78, 0x10, "Stopped");
}

extern void Screen_UpdateCurrPattern(void) {
  LCD_DrawDec(0x78, 0x20, App_CurrPattern());
}

extern void Screen_UpdateCurrInstrument(void) {
  LCD_DrawDec(0x78, 0x30, App_CurrInstrument());
}

extern void Screen_UpdateCurrStep(void) {
  LCD_DrawHex(0x78, 0x40, App_CurrStep());
}
