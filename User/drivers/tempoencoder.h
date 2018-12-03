// File:            tempoencoder.h
// Module:          TempoEncoder
// Description:     Driver for rotary encoder for adjusting tempo.
// Resources:       

#ifndef __TEMPOENCODER_H
#define __TEMPOENCODER_H

#include <stdint.h>

#define TempoEncoder_Pin1_CLK         RCC_APB2Periph_GPIOC
#define TempoEncoder_Pin1_PORT        GPIOC
#define TempoEncoder_Pin1_PIN         GPIO_Pin_6
#define TempoEncoder_Pin1_PortSource  GPIO_PortSourceGPIOC
#define TempoEncoder_Pin1_PinSource   GPIO_PinSource6

#define TempoEncoder_Pin2_CLK         RCC_APB2Periph_GPIOC
#define TempoEncoder_Pin2_PORT        GPIOC
#define TempoEncoder_Pin2_PIN         GPIO_Pin_7
#define TempoEncoder_Pin2_PortSource  GPIO_PortSourceGPIOC
#define TempoEncoder_Pin2_PinSource   GPIO_PinSource7

#define TempoEncoder_MIN              60
#define TempoEncoder_MAX              220

void TempoEncoder_Init(void);
void TempoEncoder_SetChangeHandler(void handler(int));

#endif
