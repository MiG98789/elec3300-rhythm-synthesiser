#include "stm32f10x.h"

#include "sn74hc166.h"
#include "timer.h"

static const int8_t Map[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
static void (*RisingHandler)(uint16_t rising) = 0;

static void WriteSHLD(BitAction shld) {
  GPIO_WriteBit(SN74HC166_SHLD_PORT, SN74HC166_SHLD_PIN, shld);
  Timer_Delay(13); // 181ns
}

static void PulseCLK(void) {
  GPIO_WriteBit(SN74HC166_CLK_PORT, SN74HC166_CLK_PIN, Bit_RESET);
  Timer_Delay(7); // 97ns
  GPIO_WriteBit(SN74HC166_CLK_PORT, SN74HC166_CLK_PIN, Bit_SET);
}

static int ReadQH(void) {
  return GPIO_ReadInputDataBit(SN74HC166_QH_PORT, SN74HC166_QH_PIN);
}

static uint16_t ShiftIn(void) {
  uint16_t data;
  int i;
  WriteSHLD(Bit_RESET);                   // Switch to LD mode
  PulseCLK();                             // Load data into shift register
  WriteSHLD(Bit_SET);                     // Switch to SH mode
  for (i = 0; i < 16; ++i) {
    if (i) PulseCLK();                    // Pulse clock except first time
    data |= ReadQH() << (15 - Map[i]);    // Read QH to i-th mapped bit
  }
  return data;
}

static void InitGPIO(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  
  RCC_APB2PeriphClockCmd(SN74HC166_CLK_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(SN74HC166_QH_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(SN74HC166_SHLD_CLK, ENABLE);

  GPIO_InitStruct.GPIO_Pin = SN74HC166_SHLD_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SN74HC166_SHLD_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Pin = SN74HC166_CLK_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SN74HC166_CLK_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Pin = SN74HC166_QH_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SN74HC166_QH_PORT, &GPIO_InitStruct);
}

extern void SN74HC166_Init(void) {
  InitGPIO();
}

extern void SN74HC166_Poll(void) {
  static uint16_t prevState = 0x0;
  uint16_t currState = ShiftIn();
  
  // Detect 0 to 1 transition
  uint16_t rising = currState & (currState ^ prevState);
  if (RisingHandler && rising) RisingHandler(rising);
  prevState = currState;
}

extern void SN74HC166_SetRisingHandler(void handler(uint16_t rising)) {
  RisingHandler = handler;
}
