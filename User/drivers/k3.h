// File:          k3.h
// Module:        K3
// Description:   Driver for K3 push button
// Resources:     

#ifndef __K3_H
#define __K3_H

#define K3_CLK          RCC_APB2Periph_GPIOA
#define K3_PORT         GPIOA
#define K3_PIN          GPIO_Pin_3
#define K3_PortSource   GPIO_PortSourceGPIOA
#define K3_PinSource    GPIO_PinSource3

void K3_Init(void);

#endif
