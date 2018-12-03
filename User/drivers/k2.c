#include "stm32f10x.h"

#include "k2.h"

static void (*ClickHandler)(void) = 0;

static void InitGPIO(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  
  RCC_APB2PeriphClockCmd(K2_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_InitStruct.GPIO_Pin = K2_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	// GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(K2_PORT, &GPIO_InitStruct);

  GPIO_EXTILineConfig(K2_PortSource, K2_PinSource);
}

static void InitEXTI(void) {
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
  
  EXTI_InitStruct.EXTI_Line = EXTI_Line13;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStruct);
  
  NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
}

extern void K2_Init() {
  static int init = 0;
  if (init) return;
  else init = 1;

  InitGPIO();
  InitEXTI();
}

extern void K2_SetClickHandler(void handler(void)) {
  ClickHandler = handler;
}

extern void EXTI15_10_IRQHandler(void) {
  if (EXTI_GetITStatus(EXTI_Line13)) {
    if (ClickHandler) ClickHandler();
    EXTI_ClearITPendingBit(EXTI_Line13);
  }
}
