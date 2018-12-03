// Filename:      audio.h
// Module:        Audio
// Description:   Transfers PCM samples from an audio buffer to DAC.
// Resources:     PA4 DMA2_Channel3 TIM6

#ifndef __AUDIO_H
#define __AUDIO_H

#include <stdint.h>

#define Audio_DMA_CLK       RCC_AHBPeriph_DMA2
#define Audio_DMA           DMA2_Channel3
#define Audio_DMA_IRQn      DMA2_Channel3_IRQn

void Audio_Init(void);

void Audio_SetBuffer(const uint16_t* buffer, int length);

#endif
