#include "audio.h"
#include "instruments.h"
#include "pattern.h"
#include "volume.h"

#include "lcd.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
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
    READ_PATTERN_BUTTONS();
    SEND_PATTERN_LED_COMMANDS();
    READ_VOLUME_POTENTIOMETERS();
    
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

////////////////////////////////////////////////////////////////////////////////
// K1 interrupt
////////////////////////////////////////////////////////////////////////////////

void EXTI0_IRQHandler(void) {
  if (EXTI_GetITStatus(EXTI_Line0)) { 
    CURR_INSTRUMENT = (CURR_INSTRUMENT + 1) & 0x7;
    LCD_DrawString(0x60, 0x00, "          ");
    LCD_DrawString(0x60, 0x00, INSTRUMENT_NAMES[CURR_INSTRUMENT]);
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}
