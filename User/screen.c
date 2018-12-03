#include <stdio.h>

#include "drivers/lcd.h"
#include "drivers/poll.h"
#include "drivers/tempoencoder.h"
#include "drivers/tsc2046.h"

#include "app.h"
#include "screen.h"

static void OnTouch(uint8_t x, uint8_t y) {
  char output[9] = "";
  sprintf(output, "(%d,%d)", x, y);

  LCD_DrawString(0x78, 0x60, "         ");
  LCD_DrawString(0x78, 0x60, output);
}

static void OnTempoChange(int tempoBPM) {
  char output[3] = "";
  if (tempoBPM < 100)
    sprintf(output, "%d ", tempoBPM);
  else
    sprintf(output, "%d", tempoBPM);
  
  LCD_DrawString(0x78, 0x50, output);
}

extern void Screen_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;
  
  LCD_INIT();
  Poll_Init();
  TSC2046_Init();

  LCD_DrawString(0x00, 0x00, "Mode:");
  LCD_DrawString(0x00, 0x10, "Status:");
  LCD_DrawString(0x00, 0x20, "Pattern:");
  LCD_DrawString(0x00, 0x30, "Instrument:");
  LCD_DrawString(0x00, 0x40, "Step:");
  LCD_DrawString(0x00, 0x50, "Tempo:");
  LCD_DrawString(0x00, 0x60, "Touch (X,Y):");

  LCD_DrawString(0x78, 0x50, "120");
  LCD_DrawString(0x78, 0x60, "(0,0)");

  Poll_AddHandler(Screen_Debug);
  TempoEncoder_SetChangeHandler(OnTempoChange);
  TSC2046_SetTouchHandler(OnTouch);
  
}

extern void Screen_UpdateCurrMode(void) {
  if (App_CurrMode() == App_Mode_EditPattern)
    LCD_DrawString(0x78, 0x00, "Edit Pattern");
  else
    LCD_DrawString(0x78, 0x00, "Play Pattern");
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
  LCD_DrawDec(0x78, 0x40, App_CurrStep());
}

extern void Screen_Debug(void) {
  LCD_DrawString(0x00, 0x130, "Debug:");
  LCD_DrawDec(0x78, 0x130, GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13));
}
