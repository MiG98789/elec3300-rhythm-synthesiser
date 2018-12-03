// File:            delay.h
// Module:          Delay
// Description:     Timer-based delay
// Resources:       TIM2

#ifndef __DELAY_H
#define __DELAY_H

#define Delay_TIM_CLK       RCC_APB1Periph_TIM2
#define Delay_TIM           TIM2

void Delay_Init(void);
void Delay_Cycles(uint16_t cycles);

#endif
