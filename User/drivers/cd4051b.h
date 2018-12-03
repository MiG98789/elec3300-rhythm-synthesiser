// File:          cd4051b.h
// Module:        CD4051B
// Description:   Driver for CD4051B 8-channel analog multiplexer.
// Resources:     PA6, PC8, PC9, PD2
// Dependencies:  Delay

#ifndef __CD4051B_H
#define __CD4051B_H

#include <stdint.h>

#define CD4051B_A_CLK       RCC_APB2Periph_GPIOC
#define CD4051B_A_PORT      GPIOC
#define CD4051B_A_PIN       GPIO_Pin_8

#define CD4051B_B_CLK       RCC_APB2Periph_GPIOC
#define CD4051B_B_PORT      GPIOC
#define CD4051B_B_PIN       GPIO_Pin_9

#define CD4051B_C_CLK       RCC_APB2Periph_GPIOD
#define CD4051B_C_PORT      GPIOD
#define CD4051B_C_PIN       GPIO_Pin_2

#define CD4051B_COM_CLK     RCC_APB2Periph_GPIOA
#define CD4051B_COM_PORT    GPIOA
#define CD4051B_COM_PIN     GPIO_Pin_6

#define CD4051B_ADC_CLK     RCC_APB2Periph_ADC1
#define CD4051B_ADC         ADC1
#define CD4051B_ADC_Channel ADC_Channel_6

#define CD4051B_NumChannels 8
#define CD4051B_MaxValue    0xFFF

void CD4051B_Init(void);
int16_t CD4051B_Read(int i);

#endif
