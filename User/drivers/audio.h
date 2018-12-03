// Filename:      audio.h
// Module:        Audio
// Description:   Transfers PCM samples from an audio buffer to DAC.
// Resources:     PA4 DMA2_Channel3 TIM6

#ifndef __AUDIO_H
#define __AUDIO_H

#include <stdint.h>

#define Audio_Bias 2048

void Audio_Init(const uint16_t* buffer, int length);

#endif
