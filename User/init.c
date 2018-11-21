#include "init.h"
#include "instruments.h"
#include "audio.h"

#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#include "lcd.h"
#include "misc.h"

////////////////////////////////////////////////////////////////////////////////
// GPIO init
////////////////////////////////////////////////////////////////////////////////

static void initGPIOA(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
  EXTI_InitTypeDef EXTI_InitStruct;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  // K1
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIO_EXTILineConfig(RCC_APB2Periph_GPIOA, GPIO_Pin_0);

  EXTI_InitStruct.EXTI_Line = EXTI_Line0;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStruct);
  
  NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
  
  // DAC: OUT - PA4
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void initGPIOB(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// Pattern:
  // Buttons: CLK - PB5, QH - PB6, SH/LD - PB7
  // LEDs: SRCLK - PB14, RCLCK - PB13, SER - PB12
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  // Ensure Pattern CLK is LOW
  GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
}

////////////////////////////////////////////////////////////////////////////////
// Timer init
////////////////////////////////////////////////////////////////////////////////

// Pattern timer
static void initTIM3(void) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
  TIM_TimeBaseInitStruct.TIM_Period = 1200 - 1;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 1000 - 1;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
  TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  
  NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
}

// DAC timer
static void initTIM6(void) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  
  TIM_TimeBaseInitStruct.TIM_Period = 2250 - 1; // 32kHz
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
static void initDMA2Channel3(void) {
  DMA_InitTypeDef DMA_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
  
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &DAC->DHR12R1;
  DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t) AUDIO_BUFFER;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStruct.DMA_BufferSize = STEP_SIZE;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;
  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel3, &DMA_InitStruct);
  DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE);
  DMA_Cmd(DMA2_Channel3, ENABLE);
  
  NVIC_InitStruct.NVIC_IRQChannel = DMA2_Channel3_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
}

////////////////////////////////////////////////////////////////////////////////
// DAC init
////////////////////////////////////////////////////////////////////////////////

static void initDAC1(void) {
  DAC_InitTypeDef DAC_InitStruct;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  
  DAC_InitStruct.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  
  DAC_Init(DAC_Channel_1, &DAC_InitStruct);
  DAC_DMACmd(DAC_Channel_1, ENABLE);
  DAC_SetChannel1Data(DAC_Align_12b_R, 2048);
  DAC_Cmd(DAC_Channel_1, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
// ADC init
////////////////////////////////////////////////////////////////////////////////

/*static void initADC1(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  
}*/

////////////////////////////////////////////////////////////////////////////////
// Exported
////////////////////////////////////////////////////////////////////////////////

void INIT_ALL(void) {
  LCD_INIT();
  
  initGPIOA();
  initGPIOB();

  initTIM3();
  initTIM6();

  initDMA2Channel3();
  initDAC1();
  
  TIM_Cmd(TIM3, ENABLE);
  TIM_Cmd(TIM6, ENABLE);
  
  LCD_DrawString(0x00, 0x00, "Instrument:");
  LCD_DrawString(0x00, 0x10, "Button:");
  LCD_DrawString(0x00, 0x20, "Pattern:");
  LCD_DrawString(0x00, 0x30, "Step:");
  
  LCD_DrawString(0x60, 0x00, INSTRUMENT_NAMES[0]);

}
