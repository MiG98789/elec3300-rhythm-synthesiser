#include "volume.h"
#include "helper.h"

#include "lcd.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"

////////////////////////////////////////////////////////////////////////////////
// Static
////////////////////////////////////////////////////////////////////////////////

// static const uint8_t potMapping[8] = { 5, 7, 0, 6, 3, 2, 1, 4 };
static const uint8_t potMapping[8] = { 2, 6, 5, 4, 7, 0, 3, 1 };

////////////////////////////////////////////////////////////////////////////////
// Export
////////////////////////////////////////////////////////////////////////////////

uint16_t INSTRUMENT_VOLUMES[8] = { 0 };
uint16_t MASTER_VOLUME = 0;

void READ_VOLUME_POTENTIOMETERS(void) {
  static uint8_t currPot = 0;
  BitAction A = Bit_RESET;
  BitAction B = Bit_RESET;
  BitAction C = Bit_RESET;
	uint8_t potMapIndex = 0;
  
  // COM IN/OUT - PA6 (ADC1), A - PC8, B - PC9, C - PD2
  // Master volume - PA7 (ADC2)

  // 0-7 are instrument pots
  // 8 is master volume pot
  if (currPot < 8) {
    A = (BitAction) ((currPot >> 0) & 1);
    B = (BitAction) ((currPot >> 1) & 1);
    C = (BitAction) ((currPot >> 2) & 1);
    
    GPIO_WriteBit(GPIOC, GPIO_Pin_8, A);
    GPIO_WriteBit(GPIOC, GPIO_Pin_9, B);
    GPIO_WriteBit(GPIOD, GPIO_Pin_2, C);
      
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(ADC_GetSoftwareStartConvStatus(ADC1));
    
    potMapIndex = potMapping[currPot];
    INSTRUMENT_VOLUMES[potMapIndex] = ADC_GetConversionValue(ADC1);
    
    LCD_DrawString(0x60, 0x70 + currPot*0x10, "    ");
    LCD_DrawDec(0x60, 0x70 + currPot*0x10, INSTRUMENT_VOLUMES[currPot]);
  } else {
    ADC_SoftwareStartConvCmd(ADC2, ENABLE);
    while(ADC_GetSoftwareStartConvStatus(ADC2));	
    MASTER_VOLUME = ADC_GetConversionValue(ADC2);
    
    LCD_DrawString(0x60, 0x60, "    ");
    LCD_DrawDec(0x60, 0x60, MASTER_VOLUME);
  }
  
  currPot += 1;
  if (currPot == 9) {
    currPot = 0;
  }
}
