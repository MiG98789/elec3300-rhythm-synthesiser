// File:            tempoencoder.h
// Module:          TempoEncoder
// Description:     Driver for rotary encoder for adjusting tempo.
// Resources:       

#ifndef __TEMPOENCODER_H
#define __TEMPOENCODER_H

#include <stdint.h>

#define TempoEncoder_Pin1_CLK    RCC_APB2Periph_GPIOC
#define TempoEncoder_Pin1_PORT   GPIOC
#define TempoEncoder_Pin1_PIN    GPIO_Pin_6

#define TempoEncoder_Pin2_CLK     RCC_APB2Periph_GPIOC
#define TempoEncoder_Pin2_PORT    GPIOC
#define TempoEncoder_Pin2_PIN     GPIO_Pin_7

void TempoEncoder_Init(void);
void TempoEncoder_SetChangeHandler(void handler(int));

#endif
