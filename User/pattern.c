#include "pattern.h"
#include "helper.h"

#include "lcd.h"
#include "stm32f10x_gpio.h"

////////////////////////////////////////////////////////////////////////////////
// Static
////////////////////////////////////////////////////////////////////////////////

static const uint8_t patternButtonMapping[16] = {4, 5, 6, 7, 3, 2, 1, 0, 12, 13, 14, 15, 11, 10, 9, 8};

static void CLK_Pulse() {
  GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
  Delayus(100);
  GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
}

////////////////////////////////////////////////////////////////////////////////
// Export
////////////////////////////////////////////////////////////////////////////////

// kick snare tom perc clap cowbell openhat hihat
uint16_t PATTERNS[16][8] = {
  { 0x9A9A, 0x4848, 0x0404, 0x0000, 0x0000, 0x0000, 0x2222, 0xFFFF }
};
uint8_t CURR_PATTERN = 0;
uint8_t CURR_INSTRUMENT = 0;
uint16_t CURR_STEP = 0x1;

void READ_PATTERN_BUTTONS(void) {
  static uint16_t prevResult = 0;
  uint16_t currResult = 0;
  uint8_t buttonRawIndex = 0;
	uint8_t buttonMapIndex = 0;
	uint8_t inputDataBit = 0;
  
  uint16_t xorResult = 0;
  uint16_t andResult = 0;

  // CLK:   PB5
  // QH:    PB6
  // SH/LD: PB7
    
  // Set CLK to low
  GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);

  // 2) Set SH/LD to low
  GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
		
  // 3) Set CLK to high, delayus, CLK to low
  CLK_Pulse();

  // 4) Set SH/LD to high
  GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
		
  // 5) 16-for loop, each time do step 3 and read QH
  for (; buttonRawIndex < 16; buttonRawIndex++) {
    inputDataBit = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);

    buttonMapIndex = patternButtonMapping[buttonRawIndex];
    currResult |= inputDataBit << (15 - buttonMapIndex);

    CLK_Pulse();
  }
  
  // Compare with prevResult
  // XOR prevResult and currResult, then AND the XOR value and currResult, then XOR the AND pattern
  // XOR to detect change, AND to block falling edge, XOR to toggle
  xorResult = currResult ^ prevResult;
  andResult = currResult & xorResult;
  PATTERNS[CURR_PATTERN][CURR_INSTRUMENT] ^= andResult;
  prevResult = currResult;
  
  LCD_DrawBin(0x60, 0x10, currResult);
  LCD_DrawBin(0x60, 0x20, PATTERNS[CURR_PATTERN][CURR_INSTRUMENT]);
  LCD_DrawBin(0x60, 0x30, CURR_STEP);
}

void STEP_PATTERN(void) {
  CURR_STEP = (CURR_STEP >> 1) | (CURR_STEP << 15);
}
