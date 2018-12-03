#include "k3.h"

extern void K3_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;
}
