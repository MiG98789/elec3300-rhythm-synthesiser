#include "stm32f10x.h"

#include "k1.h"

static void (*ClickHandler)(void) = 0;

static void InitGPIO(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  
  RCC_APB2PeriphClockCmd(K1_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_InitStruct.GPIO_Pin = K1_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(K1_PORT, &GPIO_InitStruct);

  GPIO_EXTILineConfig(K1_PortSource, K1_PinSource);
}

static void InitEXTI(void) {
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  EXTI_InitStruct.EXTI_Line = EXTI_Line0;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStruct);
  
  NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
}

extern void K1_Init() {
  static int init = 0;
  if (init) return;
  else init = 1;

  InitGPIO();
  InitEXTI();
}

extern void K1_SetClickHandler(void handler(void)) {
  ClickHandler = handler;
}

extern void EXTI0_IRQHandler(void) {
  if (EXTI_GetITStatus(EXTI_Line0)) {
    if (ClickHandler) ClickHandler();
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}
