#ifndef __VOLUME_H
#define __VOLUME_H

#include <stdint.h>

extern uint16_t INSTRUMENT_VOLUMES[8];
extern uint16_t MASTER_VOLUME;

void READ_VOLUME_POTENTIOMETERS(void);

#endif
