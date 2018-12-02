#include "stm32f10x.h"

#include "audio.h"

static void initGPIO(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void initDMA(const uint16_t* buffer, int bufferSize) {
  DMA_InitTypeDef DMA_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
    
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &DAC->DHR12R1;
  DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t) buffer;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStruct.DMA_BufferSize = bufferSize;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;
  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel3, &DMA_InitStruct);
  
  NVIC_InitStruct.NVIC_IRQChannel = DMA2_Channel3_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

  DMA_ITConfig(DMA2_Channel3, DMA_IT_HT | DMA_IT_TC, ENABLE);
  DMA_Cmd(DMA2_Channel3, ENABLE);
}

static void initDAC(void) {
  DAC_InitTypeDef DAC_InitStruct;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  
  DAC_InitStruct.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStruct);

  DAC_SetChannel1Data(DAC_Align_12b_L, Audio_BIAS);
  DAC_DMACmd(DAC_Channel_1, ENABLE);
  DAC_Cmd(DAC_Channel_1, ENABLE);
}

static void initTIM(void) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  
  TIM_TimeBaseInitStruct.TIM_Period = 2250 - 1; // 32kHz
  TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
  
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
  TIM_Cmd(TIM6, ENABLE);
}

void Audio_Init(const uint16_t* buffer, int bufferSize) {
  initGPIO();
  initDMA(buffer, bufferSize);
  initDAC();
  initTIM();
}