#include "stm32f10x.h"

#include "tsc2046.h"

static void InitGPIO(void) {
}

extern void TSC2046_Init(void) {
  InitGPIO();
}
