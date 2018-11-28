#include "volume.h"
#include "helper.h"

#include "lcd.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"

////////////////////////////////////////////////////////////////////////////////
// Export
////////////////////////////////////////////////////////////////////////////////

void READ_VOLUME_POTENTIOMETERS(void) {
  static uint8_t currPot = 0;
  uint8_t A = 0;
  uint8_t B = 0;
  uint8_t C = 0;
  uint8_t mask = 1;
  uint16_t result = 0;
  
  // COM IN/OUT - PA6 (ADC1), A - PC8, B - PC9, C - PD2
  // Master volume - PA7 (ADC2)

  // 0-7 are instrument pots
  // 8 is master volume pot
  if (currPot < 8) {
    A = currPot & (mask << 2);
    B = currPot & (mask << 1);
    C = currPot;
    
    GPIO_WriteBit(GPIOC, GPIO_Pin_8, A);
    GPIO_WriteBit(GPIOC, GPIO_Pin_9, B);
    GPIO_WriteBit(GPIOD, GPIO_Pin_2, C);
      
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(ADC_GetSoftwareStartConvStatus(ADC1));
    result = ADC_GetConversionValue(ADC1);
  } else {
    ADC_SoftwareStartConvCmd(ADC2, ENABLE);
    while(ADC_GetSoftwareStartConvStatus(ADC2));	
    result = ADC_GetConversionValue(ADC2);
    
    LCD_DrawString(0x60, 0x40, "     ");
    LCD_DrawDec(0x60, 0x40, result);
  }
  
  currPot += 1;
  if (currPot == 9) {
    currPot = 0;
  }
}
