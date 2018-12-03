#include "stm32f10x.h"

#include "delay.h"

// Cycle timer (72MHz)
static void InitTIM(void) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

  RCC_APB1PeriphClockCmd(Delay_TIM_CLK, ENABLE);
  
  TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(Delay_TIM, &TIM_TimeBaseInitStruct);
  
  TIM_Cmd(Delay_TIM, ENABLE);
}

extern void Delay_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;
  
  InitTIM();
}

extern void Delay_Cycles(uint16_t cycles) {
  const uint16_t start = Delay_TIM->CNT;
  while ((uint16_t) (Delay_TIM->CNT - start) < cycles);
}
