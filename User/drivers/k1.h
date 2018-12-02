// File:          k1.h
// Module:        K1
// Description:   Driver for K1 push button
// Resources:     PA0

#ifndef __K1_H
#define __K1_H

#define K1_CLK          RCC_APB2Periph_GPIOA
#define K1_PORT         GPIOA
#define K1_PIN          GPIO_Pin_0
#define K1_PortSource   GPIO_PortSourceGPIOA
#define K1_PinSource    GPIO_PinSource0

void K1_Init(void);
void K1_SetClickHandler(void handler(void));

#endif
