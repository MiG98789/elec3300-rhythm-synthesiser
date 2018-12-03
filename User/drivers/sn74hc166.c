#include "stm32f10x.h"

#include "sn74hc166.h"
#include "delay.h"
#include "poll.h"

static const int8_t Map[] = { 4, 5, 6, 7, 3, 2, 1, 0, 12, 13, 14, 15, 11, 10, 9, 8 };
static void (*RisingHandler)(uint16_t rising) = 0;

static void WriteSHLD(BitAction shld) {
  GPIO_WriteBit(SN74HC166_SHLD_PORT, SN74HC166_SHLD_PIN, shld);
  Delay_Cycles(13); // 181ns
}

static void PulseCLK(void) {
  GPIO_WriteBit(SN74HC166_CLK_PORT, SN74HC166_CLK_PIN, Bit_RESET);
  Delay_Cycles(7); // 97ns
  GPIO_WriteBit(SN74HC166_CLK_PORT, SN74HC166_CLK_PIN, Bit_SET);
}

static int ReadQH(void) {
  return GPIO_ReadInputDataBit(SN74HC166_QH_PORT, SN74HC166_QH_PIN);
}

static uint16_t ShiftIn(void) {
  uint16_t data = 0x0;
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
  static int init = 0;
  if (init) return;
  else init = 1;

  Delay_Init();
  Poll_Init();

  InitGPIO();

  Poll_AddHandler(SN74HC166_Poll);
}

extern void SN74HC166_Poll(void) {
  static uint16_t prevState = 0x0;
  uint16_t currState = ShiftIn();
  uint16_t rising = currState & (currState ^ prevState);
  if (RisingHandler && rising) RisingHandler(rising);
  prevState = currState;
}

extern void SN74HC166_SetRisingHandler(void handler(uint16_t rising)) {
  RisingHandler = handler;
}
