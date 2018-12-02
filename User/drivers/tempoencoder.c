#include "stm32f10x.h"

#include "tempoencoder.h"

static const int8_t EncoderStates[] = { 0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
static void (*EncoderHandler)(int direction) = 0;

static void InitGPIO(void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  RCC_APB2PeriphClockCmd(TempoEncoder_Pin1_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(TempoEncoder_Pin2_CLK, ENABLE);

  GPIO_InitStruct.GPIO_Pin = TempoEncoder_Pin1_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TempoEncoder_Pin1_PORT, &GPIO_InitStruct);
  GPIO_EXTILineConfig(TempoEncoder_Pin1_PortSource, TempoEncoder_Pin1_PinSource);

  GPIO_InitStruct.GPIO_Pin = TempoEncoder_Pin2_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TempoEncoder_Pin2_PORT, &GPIO_InitStruct);
}

static void InitEXTI(void) {
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct_95;

  EXTI_InitStruct.EXTI_Line = EXTI_Line6;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_Init(&EXTI_InitStruct);

  NVIC_InitStruct_95.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStruct_95.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStruct_95.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStruct_95.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct_95);
}

extern void TempoEncoder_Init(void) {
  InitGPIO();
  InitEXTI();
}

extern void TempoEncoder_SetChangeHandler(void handler(int)) {
  EncoderHandler = handler;
}

extern void EXTI9_5_IRQHandler(void) {
  static uint8_t prevState = 0;
  static uint8_t currState = 0;

  if (EXTI_GetITStatus(EXTI_Line6)) {
    prevState = prevState << 2;
    prevState |= (GPIO_ReadInputDataBit(TempoEncoder_Pin2_PORT, TempoEncoder_Pin2_PIN) & 0x03);
    currState = EncoderStates[prevState & 0x0f];

    if (EncoderHandler) EncoderHandler(currState);

    EXTI_ClearITPendingBit(EXTI_Line6);
  }
}
