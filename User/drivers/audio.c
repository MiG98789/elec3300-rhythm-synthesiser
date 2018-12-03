#include "stm32f10x.h"

#include "audio.h"

static void InitGPIO(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void InitDMA() {
  DMA_InitTypeDef DMA_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
  
  RCC_AHBPeriphClockCmd(Audio_DMA_CLK, ENABLE);
    
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &DAC->DHR12L1;
  DMA_InitStruct.DMA_MemoryBaseAddr = 0;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStruct.DMA_BufferSize = 0;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(Audio_DMA, &DMA_InitStruct);
  DMA_ITConfig(Audio_DMA, DMA_IT_TC, ENABLE);
  DMA_Cmd(Audio_DMA, ENABLE);
  
  NVIC_InitStruct.NVIC_IRQChannel = Audio_DMA_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
}

static void InitDAC(void) {
  DAC_InitTypeDef DAC_InitStruct;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  
  DAC_InitStruct.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStruct);

  DAC_DMACmd(DAC_Channel_1, ENABLE);
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_SetChannel1Data(DAC_Align_12b_L, 0x8000);
}

static void InitTIM(void) {
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

extern void Audio_Init() {
  static int init = 0;
  if (init) return;
  else init = 1;

  InitGPIO();
  InitDMA();
  InitDAC();
  InitTIM();
}

extern void Audio_SetBuffer(const uint16_t* buffer, int bufferSize) {
  DMA_Cmd(Audio_DMA, DISABLE);
  Audio_DMA->CMAR = (uint32_t) buffer;
  Audio_DMA->CNDTR = bufferSize;
  DMA_Cmd(Audio_DMA, ENABLE);
}
