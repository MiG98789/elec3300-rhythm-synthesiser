#include "stm32f10x.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "misc.h"

#include "lcd.h"
#include "drum_samples.h"

#include <math.h>

uint16_t x;

#define M_PI 3.14159265358979323846

void GPIOA_INIT(void);
void TIM6_INIT(void);
void TIM7_INIT(void);
void DAC1_INIT(void);
void DMA2_Channel3_INIT(void);

int main(void)
{
  GPIOA_INIT();
  DMA2_Channel3_INIT();
  DAC1_INIT();
  TIM6_INIT();
  TIM7_INIT();
  while (1);
}

void GPIOA_INIT(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void TIM6_INIT(void) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  TIM_TimeBaseInitStruct.TIM_Period = 1632;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
}

void TIM7_INIT(void) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  TIM_TimeBaseInitStruct.TIM_Period = 60000 - 1;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 1200 - 1;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  
  NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);
  TIM_SelectOutputTrigger(TIM7, TIM_TRGOSource_Update);
  TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM7, ENABLE);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_Init(&NVIC_InitStruct);  
}

void DAC1_INIT(void) {
  DAC_InitTypeDef DAC_InitStruct;
  DAC_InitStruct.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;
  DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  DAC_Init(DAC_Channel_1, &DAC_InitStruct);
  DAC_DMACmd(DAC_Channel_1, ENABLE);
  DAC_SetChannel1Data(DAC_Align_12b_R, 2048);
  DAC_Cmd(DAC_Channel_1, ENABLE);
}

void DMA2_Channel3_INIT(void) {
  DMA_InitTypeDef DMA_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
  
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &DAC->DHR12R1;
  DMA_InitStruct.DMA_MemoryBaseAddr = 0;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStruct.DMA_BufferSize = 17216;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;
  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel3, &DMA_InitStruct);
  DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE);
  DMA_Cmd(DMA2_Channel3, ENABLE);
  
  NVIC_InitStruct.NVIC_IRQChannel = DMA2_Channel3_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
}

void TIM7_IRQHandler(void) {
  if (TIM_GetITStatus(TIM7, TIM_IT_Update)) {
    static int i = 0;
    DMA_Cmd(DMA2_Channel3, DISABLE);
    DMA2_Channel3->CMAR = (uint32_t) DRUM_DATA[i];
    DMA2_Channel3->CNDTR = DRUM_SIZE[i];
    DMA_Cmd(DMA2_Channel3, ENABLE);
    TIM_Cmd(TIM6, ENABLE);
    i = (i + 1) & 0x7;
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
  }
}

void DMA2_Channel3_IRQHandler(void) {
  if (DMA_GetITStatus(DMA2_IT_TC3)) {
    TIM_Cmd(TIM6, DISABLE);
    DMA_Cmd(DMA2_Channel3, DISABLE);
    DAC_SetChannel1Data(DAC_Align_12b_R, 2048);
    DMA_ClearITPendingBit(DMA2_IT_TC3);
  }
}
