// File:          sn74hc166.h
// Module:        SN74HC166
// Description:   Driver for SN74HC166 shift register.
// Resources:     PA2, PB6, PB7
// Dependencies:  Delay, Poll

#ifndef __SN74HC166_H
#define __SN74HC166_H

#include <stdint.h>

#define SN74HC166_SHLD_CLK    RCC_APB2Periph_GPIOB
#define SN74HC166_SHLD_PORT   GPIOB
#define SN74HC166_SHLD_PIN    GPIO_Pin_7

#define SN74HC166_CLK_CLK     RCC_APB2Periph_GPIOA
#define SN74HC166_CLK_PORT    GPIOA
#define SN74HC166_CLK_PIN     GPIO_Pin_2

#define SN74HC166_QH_CLK      RCC_APB2Periph_GPIOB
#define SN74HC166_QH_PORT     GPIOB
#define SN74HC166_QH_PIN      GPIO_Pin_6

void SN74HC166_Init(void);
void SN74HC166_Poll(void);
void SN74HC166_SetRisingHandler(void handler(uint16_t rising));

#endif
