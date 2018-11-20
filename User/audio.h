#ifndef __AUDIO_H
#define __AUDIO_H

#include <stdint.h>

extern const uint32_t AUDIO_BUFFER_SIZE;
extern int16_t* const AUDIO_BUFFER;
extern const int16_t STEP_SIZE;

void STEP_AUDIO(void);

#endif
