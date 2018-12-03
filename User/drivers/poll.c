#include "stm32f10x.h"

#include "poll.h"

static void (*Handlers[Poll_MaxHandlers])(void) = { 0 };
static int NumHandlers = 0;

static void InitTIM(void) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
  
  RCC_APB1PeriphClockCmd(Poll_TIM_CLK, ENABLE);
  
  NVIC_InitStruct.NVIC_IRQChannel = Poll_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
  
  TIM_TimeBaseInitStruct.TIM_Period = 2400 - 1;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 1000 - 1;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(Poll_TIM, &TIM_TimeBaseInitStruct);
  
  TIM_SelectOutputTrigger(Poll_TIM, TIM_TRGOSource_Update);
  TIM_ITConfig(Poll_TIM, TIM_IT_Update, ENABLE);
  TIM_Cmd(Poll_TIM, ENABLE);
}

extern void Poll_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;

  InitTIM();
}

extern void Poll_AddHandler(void handler(void)) {
  Handlers[NumHandlers++] = handler;
}

extern void TIM3_IRQHandler(void) {
  if (TIM_GetITStatus(TIM3, TIM_IT_Update)) {
    int i;
    for (i = 0; i < NumHandlers; ++i)
      Handlers[i]();
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
}
