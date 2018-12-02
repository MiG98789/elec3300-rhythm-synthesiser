// File:          k2.h
// Module:        K2
// Description:   Driver for K2 push button
// Resources:     PC13

#ifndef __K2_H
#define __K2_H

#define K2_CLK          RCC_APB2Periph_GPIOC
#define K2_PORT         GPIOC
#define K2_PIN          GPIO_Pin_13
#define K2_PortSource   GPIO_PortSourceGPIOC
#define K2_PinSource    GPIO_PinSource13

void K2_Init(void);
void K2_SetClickHandler(void handler(void));

#endif
