#include "audio.h"
#include "pattern.h"

#include "lcd.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

////////////////////////////////////////////////////////////////////////////////
// DMA interrupt handler
////////////////////////////////////////////////////////////////////////////////

void DMA2_Channel3_IRQHandler(void) {
  if (DMA_GetITStatus(DMA2_IT_TC3)) {
    STEP_PATTERN();
    STEP_AUDIO();
    DMA_ClearITPendingBit(DMA2_IT_TC3);
  }
}

////////////////////////////////////////////////////////////////////////////////
// Pattern interrupt
////////////////////////////////////////////////////////////////////////////////

void TIM3_IRQHandler(void) {	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update)) {
    // READ_PATTERN_BUTTONS();

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
