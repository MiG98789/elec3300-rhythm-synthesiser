#include "stm32f10x.h"

#include "tsc2046.h"

static void InitGPIO(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  
  RCC_APB2PeriphClockCmd(TSC2046_DCLK_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(TSC2046_CS_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(TSC2046_DIN_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(TSC2046_DOUT_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(TSC2046_PENIRQ_CLK, ENABLE);

  GPIO_InitStruct.GPIO_Pin = TSC2046_DCLK_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TSC2046_DCLK_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = TSC2046_CS_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TSC2046_CS_PORT, &GPIO_InitStruct);

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

  GPIO_EXTILineConfig(TSC2046_PENIRQ_PortSource, TSC2046_PENIRQ_PinSource);
}

static void InitEXTI(void) {
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  EXTI_InitStruct.EXTI_Line = EXTI_Line4;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStruct);

  NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
}

extern void TSC2046_Init(void) {
  InitGPIO();
  InitEXTI();
}

extern void EXTI4_IRQHandler(void) {
  if (EXTI_GetITStatus(EXTI_Line4)) {
    EXTI_ClearITPendingBit(EXTI_Line4);
  }
}
