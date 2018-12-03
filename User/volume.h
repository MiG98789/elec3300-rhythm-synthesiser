// File:            volume.h
// Module:          Volume
// Dependencies:    CD4051B, MasterVolumePot
// Description:     Manages master and instrument volumes

#ifndef __VOLUME_H
#define __VOLUME_H

#include <stdint.h>

void Volume_Init(void);
int16_t Volume_MasterVolume(void);
int16_t Volume_InstrumentVolume(int i);

#endif
