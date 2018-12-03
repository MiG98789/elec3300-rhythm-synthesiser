#include "drivers/cd4051b.h"
#include "drivers/mastervolumepot.h"

#include "volume.h"

extern void Volume_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;

  CD4051B_Init();
  MasterVolumePot_Init();
}

extern int16_t Volume_MasterVolume(void) {
  return MasterVolumePot_Read();
}

extern int16_t Volume_InstrumentVolume(int i) {
  return CD4051B_Read(i);
}
