#include "stm32f10x.h"

#include "delay.h"
#include "poll.h"
#include "tsc2046.h"

static void (*TouchHandler)(uint8_t x, uint8_t y) = 0;

static void PulseDCLK(void) {
  GPIO_WriteBit(TSC2046_DCLK_PORT, TSC2046_DCLK_PIN, Bit_SET);
  Delay_Cycles(14);
  GPIO_WriteBit(TSC2046_DCLK_PORT, TSC2046_DCLK_PIN, Bit_RESET);
}

static uint8_t ReadTouchscreen(uint8_t controlByte) {
  /*
       Control bit def | Value | Function
     ------------------+-------+-----------------------
       Start           |   1   | Must initialise as 1
       Addressing (A2) |1/0/0/1| Measure X/Y/Z1/Z2
       Addressing (A1) |0/0/1/0| Measure X/Y/Z1/Z2
       Addressing (A0) |1/1/1/0| Measure X/Y
       Mode            |   1   | 8-bit resolution
       SER/DFR         |   0   | Differential mode
       Power-Down (PD1)|   0   | 
       Power-Down (PD0)|   0   |
  */
  uint8_t i = 0;
  uint8_t coord = 0;

  for (i = 0; i < 8; i++) {
    if (controlByte & (0x80 >> i)) {
      GPIO_WriteBit(TSC2046_DIN_PORT, TSC2046_DIN_PIN, Bit_SET);
    } else {
      GPIO_WriteBit(TSC2046_DIN_PORT, TSC2046_DIN_PIN, Bit_RESET);
    }
    PulseDCLK();
  }

  Delay_Cycles(28);

  for (i = 0; i < 8; i++) {
    PulseDCLK();
    coord = coord << 1;
    coord |= GPIO_ReadInputDataBit(TSC2046_DOUT_PORT, TSC2046_DOUT_PIN);
  }

  Delay_Cycles(28);

  return coord;
}

static void OnPoll(void) {
  static uint8_t x = 0;
  static uint8_t y = 0;

  x = ReadTouchscreen(0xd8);
  y = ReadTouchscreen(0x98);

  if (x == 0 || y == 255) {
    x = 0;
    y = 255;
  }

  if (TouchHandler) TouchHandler(x, y);
}

static void InitGPIO(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  
  RCC_APB2PeriphClockCmd(TSC2046_DCLK_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(TSC2046_CS_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(TSC2046_DIN_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(TSC2046_DOUT_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(TSC2046_PENIRQ_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitStruct.GPIO_Pin = TSC2046_DCLK_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TSC2046_DCLK_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = TSC2046_CS_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TSC2046_CS_PORT, &GPIO_InitStruct);
  GPIO_WriteBit(TSC2046_CS_PORT, TSC2046_CS_PIN, Bit_RESET);

  GPIO_InitStruct.GPIO_Pin = TSC2046_DIN_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TSC2046_DIN_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = TSC2046_DOUT_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TSC2046_DOUT_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = TSC2046_PENIRQ_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TSC2046_PENIRQ_PORT, &GPIO_InitStruct);
}

extern void TSC2046_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;

  Delay_Init();

  InitGPIO();

  Poll_AddHandler(OnPoll);
}

extern void TSC2046_SetTouchHandler(void handler(uint8_t, uint8_t)) {
  TouchHandler = handler;
}
