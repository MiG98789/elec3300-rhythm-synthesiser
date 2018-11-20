#ifndef __PATTERN_H
#define __PATTERN_H

#include <stdint.h>

extern uint16_t PATTERNS[16][8];
extern uint8_t CURR_PATTERN;
extern uint8_t CURR_INSTRUMENT;
extern uint16_t CURR_STEP;

void READ_PATTERN_BUTTONS(void);
void STEP_PATTERN(void);

#endif
