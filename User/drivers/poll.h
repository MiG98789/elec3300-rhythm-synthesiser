// File:                poll.h
// Module:              Poll
// Description:         Registers handlers to be called in polling loop
// Resources:           TIM3

#ifndef __POLL_H
#define __POLL_H

#define Poll_TIM_CLK      RCC_APB1Periph_TIM3
#define Poll_TIM          TIM3

#define Poll_IRQn         TIM3_IRQn

#define Poll_MaxHandlers  64

void Poll_Init(void);
void Poll_AddHandler(void handler(void));

#endif
