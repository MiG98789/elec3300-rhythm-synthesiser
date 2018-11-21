#ifndef __AUDIO_H
#define __AUDIO_H

#include <stdint.h>

extern int16_t AUDIO_BUFFER[8000];
extern int16_t STEP_SIZE;

void STEP_AUDIO(void);

#endif
