#include "stm32f10x.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_tim.h"

#include "lcd.h"

void RCC_INIT(void);
void GPIO_INIT(void);
void TIM_INIT(void);
void DAC_INIT(void);

int main(void)
{
  RCC_INIT();
  GPIO_INIT();
  TIM_INIT();
  DAC_INIT();
  
  while (1);
}

void RCC_INIT(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
}

void GPIO_INIT(void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void TIM_INIT(void) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

  TIM_TimeBaseInitStruct.TIM_Period = 10226;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 3;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
  TIM_Cmd(TIM6, ENABLE);
  
}

void DAC_INIT(void) {
  DAC_InitTypeDef DAC_InitStruct;
  
  DAC_InitStruct.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
  DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;
  DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStruct);
  DAC_Cmd(DAC_Channel_1, ENABLE);
}
