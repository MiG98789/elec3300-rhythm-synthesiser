// File:          mastervolumepot.h
// Module:        MasterVolumePot
// Dependencies:  Poll
// Description:   Driver for master volume potentiometer
// Resources:     PA7

#ifndef __MASTERVOLUMEPOT_H
#define __MASTERVOLUMEPOT_H

#include <stdint.h>

#define MasterVolumePot_CLK         RCC_APB2Periph_GPIOA
#define MasterVolumePot_PORT        GPIOA
#define MasterVolumePot_PIN         GPIO_Pin_7

#define MasterVolumePot_ADC_CLK     RCC_APB2Periph_ADC2
#define MasterVolumePot_ADC         ADC2
#define MasterVolumePot_ADC_Channel ADC_Channel_7

#define MasterVolumePot_MaxValue  0xFFF

void MasterVolumePot_Init(void);
int16_t MasterVolumePot_Read(void);

#endif
