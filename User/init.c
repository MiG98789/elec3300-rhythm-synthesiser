#include "init.h"

#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "lcd.h"
#include "misc.h"

////////////////////////////////////////////////////////////////////////////////
// Init structs
////////////////////////////////////////////////////////////////////////////////

static DAC_InitTypeDef DAC_InitStruct;
static DMA_InitTypeDef DMA_InitStruct;
static GPIO_InitTypeDef GPIO_InitStruct;
static NVIC_InitTypeDef NVIC_InitStruct;
static TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

////////////////////////////////////////////////////////////////////////////////
// GPIO init
////////////////////////////////////////////////////////////////////////////////

static void GPIOA_INIT(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  // DAC: OUT - PA4
  GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void GPIOB_INIT(void) {  
  GPIO_InitTypeDef GPIO_InitStructure;
	
	// Pattern:
  // Buttons: CLK - PB5, QH - PB6, SH/LD - PB7
  // LEDs: SRCLK - PB14, RCLCK - PB13, SER - PB12
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  // Ensure Pattern CLK is LOW
  GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
}

////////////////////////////////////////////////////////////////////////////////
// Timer init
////////////////////////////////////////////////////////////////////////////////

// Tempo timer
static void TIM2_INIT(void) {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  
  TIM_TimeBaseInitStruct.TIM_Period = 9000 - 1;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 1000 - 1;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  
  NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
}

// Pattern timer
static void TIM3_INIT(void) {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
  TIM_TimeBaseInitStruct.TIM_Period = 1200 - 1;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 1000 - 1;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
  TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
  
  NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
}

// Playback timer
static void TIM6_INIT(void) {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
  TIM_TimeBaseInitStruct.TIM_Period = 1632;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
}

////////////////////////////////////////////////////////////////////////////////
// DMA init
////////////////////////////////////////////////////////////////////////////////

// DMA for DAC1
static void DMA2_Channel3_INIT(void) {
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
  
  DMA_StructInit(&DMA_InitStruct);
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
}

////////////////////////////////////////////////////////////////////////////////
// DAC init
////////////////////////////////////////////////////////////////////////////////

static void DAC1_INIT(void) {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  
  DAC_StructInit(&DAC_InitStruct);
  DAC_InitStruct.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  
  DAC_Init(DAC_Channel_1, &DAC_InitStruct);
  DAC_DMACmd(DAC_Channel_1, ENABLE);
  DAC_SetChannel1Data(DAC_Align_12b_R, 2048);
  DAC_Cmd(DAC_Channel_1, ENABLE);
}

void init_all(void) {
  LCD_INIT();
  
  GPIOA_INIT();
  GPIOB_INIT();

  TIM2_INIT();
  TIM3_INIT();
  TIM6_INIT();

  DMA2_Channel3_INIT();
  DAC1_INIT();
}
