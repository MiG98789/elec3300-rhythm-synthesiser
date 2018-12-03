#include "stm32f10x.h"

#include "rgbled.h"

static void InitGPIO(void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  RCC_APB2PeriphClockCmd(RGBLED_R_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RGBLED_G_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RGBLED_B_CLK, ENABLE);

  GPIO_InitStruct.GPIO_Pin = RGBLED_R_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RGBLED_R_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Pin = RGBLED_G_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RGBLED_G_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Pin = RGBLED_B_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RGBLED_B_PORT, &GPIO_InitStruct);
}

extern void RGBLED_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;

  InitGPIO();

  RGBLED_Clear();
}

extern void RGBLED_Clear(void) {
  GPIO_WriteBit(RGBLED_R_PORT, RGBLED_R_PIN, Bit_SET);
  GPIO_WriteBit(RGBLED_G_PORT, RGBLED_G_PIN, Bit_SET);
  GPIO_WriteBit(RGBLED_B_PORT, RGBLED_B_PIN, Bit_SET);
}

extern void RGBLED_SetColor(RGBLED_Color color) {
  switch (color) {
    case RGBLED_R:
      GPIO_WriteBit(RGBLED_R_PORT, RGBLED_R_PIN, Bit_RESET);
      GPIO_WriteBit(RGBLED_G_PORT, RGBLED_G_PIN, Bit_SET);
      GPIO_WriteBit(RGBLED_B_PORT, RGBLED_B_PIN, Bit_SET);
      break;

    case RGBLED_G:
      GPIO_WriteBit(RGBLED_R_PORT, RGBLED_R_PIN, Bit_SET);
      GPIO_WriteBit(RGBLED_G_PORT, RGBLED_G_PIN, Bit_RESET);
      GPIO_WriteBit(RGBLED_B_PORT, RGBLED_B_PIN, Bit_SET);
      break;

    case RGBLED_B:
      GPIO_WriteBit(RGBLED_R_PORT, RGBLED_R_PIN, Bit_SET);
      GPIO_WriteBit(RGBLED_G_PORT, RGBLED_G_PIN, Bit_SET);
      GPIO_WriteBit(RGBLED_B_PORT, RGBLED_B_PIN, Bit_RESET);
      break;

    default:
      break;
  }
}
