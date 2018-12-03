#include "drivers/lcd.h"
#include "drivers/poll.h"

#include "app.h"
#include "screen.h"

extern void Screen_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;
  
  LCD_INIT();
  Poll_Init();

  LCD_DrawString(0x00, 0x00, "Mode:");
  LCD_DrawString(0x00, 0x10, "Status:");
  LCD_DrawString(0x00, 0x20, "Pattern:");
  LCD_DrawString(0x00, 0x30, "Instrument:");
  LCD_DrawString(0x00, 0x40, "Step:");
  
  Poll_AddHandler(Screen_Debug);
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
