#include "pattern.h"
#include "helper.h"

#include "lcd.h"
#include "stm32f10x_gpio.h"

////////////////////////////////////////////////////////////////////////////////
// Define
////////////////////////////////////////////////////////////////////////////////

#ifndef __PATTERN_C

#define Pattern_CLK_Pin   GPIO_Pin_5
#define Pattern_QH_Pin    GPIO_Pin_6
#define Pattern_SHLD_Pin  GPIO_Pin_7

#define LED_SER_Pin       GPIO_Pin_12
#define LED_RCLK_Pin      GPIO_Pin_13
#define LED_SRCLK_Pin     GPIO_Pin_14

#endif

////////////////////////////////////////////////////////////////////////////////
// Static
////////////////////////////////////////////////////////////////////////////////

static const uint8_t ledMapping[16] = {14, 13, 12, 11, 10, 9, 8, 15, 6, 5, 4, 3, 2, 1, 0, 7};
static const uint8_t patternButtonMapping[16] = {4, 5, 6, 7, 3, 2, 1, 0, 12, 13, 14, 15, 11, 10, 9, 8};

static uint16_t prevStep = 0;
static uint8_t ledStepCounter = 0;
static uint8_t ledPulseTime = 3;

static void CLK_Pulse(uint16_t GPIO_Pin) {
  GPIO_WriteBit(GPIOB, GPIO_Pin, Bit_SET);
  Delayus(100);
  GPIO_WriteBit(GPIOB, GPIO_Pin, Bit_RESET);
}

////////////////////////////////////////////////////////////////////////////////
// Export
////////////////////////////////////////////////////////////////////////////////

// kick snare tom perc clap cowbell openhat hihat
uint16_t PATTERNS[16][8] = {
  { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF }
  // { 0x9A9A, 0x4848, 0x0404, 0x0000, 0x0000, 0x0000, 0x2222, 0xFFFF }
};
uint8_t CURR_PATTERN = 0;
uint8_t CURR_INSTRUMENT = 0;
uint16_t CURR_STEP = 0x1;

void READ_PATTERN_BUTTONS(void) {
  int i;
  static uint16_t prevResult = 0;
  uint16_t currResult = 0;
  uint8_t buttonRawIndex = 0;
	uint8_t buttonMapIndex = 0;
	uint8_t inputDataBit = 0;
  
  uint16_t xorResult = 0;
  uint16_t andResult = 0;
    
  // Set CLK to low
  GPIO_WriteBit(GPIOB, Pattern_CLK_Pin, Bit_RESET);

  // Set SH/LD to low
  GPIO_WriteBit(GPIOB, Pattern_SHLD_Pin, Bit_RESET);
		
  // Set CLK to high, delayus, CLK to low
  CLK_Pulse(Pattern_CLK_Pin);

  // Set SH/LD to high
  GPIO_WriteBit(GPIOB, Pattern_SHLD_Pin, Bit_SET);
		
    // 16-for loop, each time: pulse CLK and read QH
  for (; buttonRawIndex < 16; buttonRawIndex++) {
    inputDataBit = GPIO_ReadInputDataBit(GPIOB, Pattern_QH_Pin);

    buttonMapIndex = patternButtonMapping[buttonRawIndex];
    currResult |= inputDataBit << (15 - buttonMapIndex);

    CLK_Pulse(Pattern_CLK_Pin);
  }
  
  // Compare with prevResult
  // XOR prevResult and currResult, then AND the XOR value and currResult, then XOR the AND pattern
  // XOR to detect change, AND to block falling edge, XOR to toggle
  xorResult = currResult ^ prevResult;
  andResult = currResult & xorResult;
  PATTERNS[CURR_PATTERN][CURR_INSTRUMENT] ^= andResult;
  prevResult = currResult;
  
<<<<<<< Updated upstream
  LCD_DrawBin(0x60, 0x20, currResult);
  LCD_DrawBin(0x60, 0x30, PATTERNS[CURR_PATTERN][CURR_INSTRUMENT]);
  LCD_DrawBin(0x60, 0x40, CURR_STEP);
=======
  LCD_DrawBin(0x60, 0x10, currResult);
  LCD_DrawBin(0x60, 0x20, PATTERNS[CURR_PATTERN][CURR_INSTRUMENT]);
  LCD_DrawBin(0x60, 0x30, CURR_STEP);
  
  if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)) {
    for (i = 0; i < 16; ++i)
      PATTERNS[CURR_PATTERN][i] = 0;
  }
>>>>>>> Stashed changes
}

void SEND_PATTERN_LED_COMMANDS(void) {
  uint8_t ledRawIndex = 0;
  uint8_t ledMapIndex = 0;
  uint16_t currPattern = PATTERNS[CURR_PATTERN][CURR_INSTRUMENT];
  
  // Set RCLK and SRCLK to low
  GPIO_WriteBit(GPIOB, LED_RCLK_Pin, Bit_RESET);
  GPIO_WriteBit(GPIOB, LED_SRCLK_Pin, Bit_RESET);
  
  if (prevStep != CURR_STEP) {
    prevStep = CURR_STEP;
    ledStepCounter = ledPulseTime;
  }

  if (ledStepCounter > 0) {
    currPattern ^= CURR_STEP;
    ledStepCounter--;
  }
  
  // 16-for loop, each time: write to SER and pulse SRCLK
  for (; ledRawIndex < 16; ledRawIndex++) {
    ledMapIndex = ledMapping[ledRawIndex];
    
    if (
      ((currPattern << ledMapIndex) & 0x8000) // No pulse 
    ) {
      GPIO_WriteBit(GPIOB, LED_SER_Pin, Bit_SET);
    } else {
      GPIO_WriteBit(GPIOB, LED_SER_Pin, Bit_RESET);
    }
    
    CLK_Pulse(LED_SRCLK_Pin);
  }
  
  // Pulse RCLK to activate register
  CLK_Pulse(LED_RCLK_Pin);
}

void PATTERN_TICK(void) {
  CURR_STEP = (CURR_STEP >> 1) | (CURR_STEP << 15);
}
