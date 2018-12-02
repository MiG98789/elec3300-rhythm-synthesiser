// File:          sn74hc595.h
// Module:        SN74HC595
// Description:   Driver for SN74HC595 shift register.
// Resources:     PB12, PB13, PB14

#ifndef __SN74HC595_H
#define __SN74HC595_H

#define SN74HC595_SRCLK_CLK     RCC_APB2Periph_GPIOB
#define SN74HC595_SRCLK_PORT    GPIOB
#define SN74HC595_SRCLK_PIN     GPIO_Pin_14

#define SN74HC595_RCLK_CLK      RCC_APB2Periph_GPIOB
#define SN74HC595_RCLK_PORT     GPIOA
#define SN74HC595_RCLK_PIN      GPIO_Pin_13

#define SN74HC595_SER_CLK       RCC_APB2Periph_GPIOB
#define SN74HC595_SER_PORT      GPIOB
#define SN74HC595_SER_PIN       GPIO_Pin_12

void SN74HC595_Init(void);
void SN74HC595_Poll(void);

// Continuous
void SN74HC595_SetState(uint16_t state);

// Periodic
void SN74HC595_SetFlash(uint16_t flash);

// Momentary
void SN74HC595_SetBlink(uint16_t blink);

#endif
