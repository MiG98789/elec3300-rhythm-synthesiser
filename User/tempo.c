#include "tempo.h"

extern void Tempo_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;
}
