#include "stm32f10x.h"

#include "tempoencoder.h"

static const int8_t EncoderStates[] = { 0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
static int TempoBPM = 120;
static void (*EncoderHandler)(int value) = 0;

static int ClampTempoBPM(int tempo) {
  return (tempo > TempoEncoder_MAX ? TempoEncoder_MAX : (tempo < TempoEncoder_MIN ? TempoEncoder_MIN : tempo));
}

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
  GPIO_EXTILineConfig(TempoEncoder_Pin2_PortSource, TempoEncoder_Pin2_PinSource);
}

static void InitEXTI(void) {
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct_95;

  EXTI_InitStruct.EXTI_Line = EXTI_Line6 | EXTI_Line7;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_Init(&EXTI_InitStruct);

  NVIC_InitStruct_95.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStruct_95.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStruct_95.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStruct_95.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct_95);
}

extern void TempoEncoder_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;

  InitGPIO();
  InitEXTI();
}

extern void TempoEncoder_SetChangeHandler(void handler(int)) {
  EncoderHandler = handler;
}

extern void EXTI9_5_IRQHandler(void) {
  static int currState = 0;
  static int prevState = 0;

  if (EXTI_GetITStatus(EXTI_Line6 | EXTI_Line7)) {
    currState = (GPIO_ReadInputDataBit(TempoEncoder_Pin2_PORT, TempoEncoder_Pin2_PIN)) << 1;
    currState |= GPIO_ReadInputDataBit(TempoEncoder_Pin1_PORT, TempoEncoder_Pin1_PIN);
    prevState = ((prevState << 2) + currState) & 0x0f;
    TempoBPM += EncoderStates[prevState];
    TempoBPM = ClampTempoBPM(TempoBPM);

    if (EncoderHandler) EncoderHandler(TempoBPM);

    EXTI_ClearITPendingBit(EXTI_Line6);
    EXTI_ClearITPendingBit(EXTI_Line7);
  }
}
