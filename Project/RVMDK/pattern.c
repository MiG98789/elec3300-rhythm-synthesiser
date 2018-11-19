#include "pattern.h"

#include <stdio.h>

#include "stm32f10x_gpio.h"

#include "helper.h"
#include "lcd.h"

const int PATTERN_BUTTON_MAPPING[16] = {0, 5, 6, 7, 1, 4, 3, 2, 12, 13, 14, 15, 11, 10, 9, 8};

static void CLK_Pulse(int duration) {
  GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
  Delayus(duration);
  GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
}

void readPatternButtons(void) {
  uint16_t currResult = 0;
  uint8_t buttonRawIndex = 0;
	uint8_t buttonMapIndex = 0;
	uint8_t inputDataBit = 0;
	char output[17];

  // CLK:   PB5
  // QH:    PB6
  // SH/LD: PB7
    
  // Set CLK to low
  GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);

  // 2) Set SH/LD to low
  GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
		
  // 3) Set CLK to high, delayus, CLK to low
  CLK_Pulse(10000);

  // 4) Set SH/LD to high
  GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
		
  // 5) 16-for loop, each time do step 3 and read QH
  for (; buttonRawIndex < 16; buttonRawIndex++) {
    inputDataBit = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);

    currResult = currResult << 1;
    currResult |= inputDataBit;

    buttonMapIndex = PATTERN_BUTTON_MAPPING[buttonRawIndex];
    output[buttonMapIndex] = inputDataBit + '0';

    CLK_Pulse(10000);
  }
  output[16] = '\0';
  LCD_DrawString(10, 10, output);
}
