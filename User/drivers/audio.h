// Filename:      audio.h
// Module:        Audio
// Description:   Transfers PCM samples from an audio buffer to DAC.
// GPIO Pins:     PA4 (DAC1 Output)

#ifndef __AUDIO_H
#define __AUDIO_H

#include <stdint.h>

#define Audio_BIAS 2048

void Audio_Init(const uint16_t* buffer, int length);

#endif
