// File:            tsc2046.h
// Module:          TSC2046
// Description:     Driver for TSC2046 touchscreen controller
// Resources:       

#ifndef __TSC2046_H
#define __TSC2046_H

#include <stdint.h>

#define TSC2046_DCLK_CLK            RCC_APB2Periph_GPIOE
#define TSC2046_DCLK_PORT           GPIOE
#define TSC2046_DCLK_PIN            GPIO_Pin_0

#define TSC2046_CS_CLK              RCC_APB2Periph_GPIOD
#define TSC2046_CS_PORT             GPIOD
#define TSC2046_CS_PIN              GPIO_Pin_13

#define TSC2046_DIN_CLK             RCC_APB2Periph_GPIOE
#define TSC2046_DIN_PORT            GPIOE
#define TSC2046_DIN_PIN             GPIO_Pin_2

#define TSC2046_DOUT_CLK            RCC_APB2Periph_GPIOE
#define TSC2046_DOUT_PORT           GPIOE
#define TSC2046_DOUT_PIN            GPIO_Pin_3

#define TSC2046_PENIRQ_CLK          RCC_APB2Periph_GPIOE
#define TSC2046_PENIRQ_PORT         GPIOE
#define TSC2046_PENIRQ_PIN          GPIO_Pin_4
#define TSC2046_PENIRQ_PortSource   GPIO_PortSourceGPIOE
#define TSC2046_PENIRQ_PinSource    GPIO_PinSource4

void TSC2046_Init(void);
void TSC2046_SetTouchHandler(void handler(uint8_t, uint8_t));

#endif
