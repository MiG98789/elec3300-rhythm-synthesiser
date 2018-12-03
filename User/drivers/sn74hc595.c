#include "stm32f10x.h"

#include "sn74hc595.h"
#include "delay.h"
#include "poll.h"

static const int8_t Map[] = { 14, 13, 12, 11, 10, 9, 8, 15, 6, 5, 4, 3, 2, 1, 0, 7 };

static uint16_t State = 0x0;
static uint16_t Flash = 0x0;
static uint16_t Blink = 0x0;

static const uint16_t FlashPeriod = 15;
static uint16_t FlashCounter = 0x0;
static uint16_t FlashEnable = 0xFFFF;

static void PulseSRCLK(void) {
  GPIO_WriteBit(SN74HC595_SRCLK_PORT, SN74HC595_SRCLK_PIN, Bit_RESET);
  Delay_Cycles(9); // 125ns
  GPIO_WriteBit(SN74HC595_SRCLK_PORT, SN74HC595_SRCLK_PIN, Bit_SET);
}

static void PulseRCLK(void) {
  GPIO_WriteBit(SN74HC595_RCLK_PORT, SN74HC595_RCLK_PIN, Bit_RESET);
  Delay_Cycles(7); // 97ns
  GPIO_WriteBit(SN74HC595_RCLK_PORT, SN74HC595_RCLK_PIN, Bit_SET);
}

static void WriteSER(BitAction ser) {
  GPIO_WriteBit(SN74HC595_SER_PORT, SN74HC595_SER_PIN, ser);
}

static void ShiftOut(uint16_t data) {
  int i;
  for (i = 0; i < 16; ++i) {
    if (data & (0x8000 >> Map[i]))
      WriteSER(Bit_SET);
    else
      WriteSER(Bit_RESET);
    PulseSRCLK();
  }
  PulseRCLK();
}

static void InitGPIO(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  
  RCC_APB2PeriphClockCmd(SN74HC595_SRCLK_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(SN74HC595_RCLK_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(SN74HC595_SER_CLK, ENABLE);

  GPIO_InitStruct.GPIO_Pin = SN74HC595_SRCLK_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SN74HC595_SRCLK_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Pin = SN74HC595_RCLK_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SN74HC595_RCLK_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Pin = SN74HC595_SER_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SN74HC595_SER_PORT, &GPIO_InitStruct);
}

extern void SN74HC595_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;
  
  Delay_Init();

  State = 0x0;
  Flash = 0x0;
  Blink = 0x0;
  
  InitGPIO();
  
  ShiftOut(0x0);
  
  Poll_AddHandler(SN74HC595_Poll);
}

extern void SN74HC595_Poll(void) {
  uint16_t output = State;
  output |= FlashEnable & Flash;
  output ^= Blink;
  ShiftOut(output);
  
  Blink = 0x0;
  if (++FlashCounter == FlashPeriod) {
    FlashCounter = 0;
    FlashEnable = ~FlashEnable;
  }
}

void SN74HC595_SetState(uint16_t state) {
  State = state;
}

void SN74HC595_SetFlash(uint16_t flash) {
  Flash = flash;
  FlashCounter = 0;
  FlashEnable = 0xFFFF;
}

void SN74HC595_SetBlink(uint16_t blink) {
  Blink = blink;
}
