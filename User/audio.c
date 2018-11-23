#include "audio.h"
#include "instruments.h"
#include "pattern.h"

#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"

////////////////////////////////////////////////////////////////////////////////
// Static
////////////////////////////////////////////////////////////////////////////////

static const uint8_t mixSize = 16;
static const int16_t* mixData[mixSize] = { 0 };
static uint16_t mixCounter[mixSize] = { 0 };

static int16_t masterVolume = 4096;

static uint32_t min_element(const uint16_t* const first, const uint16_t* const last) {
  const uint16_t* smallest = first;
  const uint16_t* it = first;
  while (++it != last)
    if (*it < *smallest)
      smallest = it;
  return smallest - first;
}

static void clamp12(int32_t* sample) {
  *sample = *sample > 2047 ? 2047 : *sample;
  *sample = *sample < -2048 ? -2048 : *sample;
}

static void updateMix(void) {
  uint32_t i, j;
  for (i = 0; i < INSTRUMENT_COUNT; ++i) {
    if (PATTERNS[CURR_PATTERN][i] & CURR_STEP) {
      j = min_element(mixCounter, mixCounter + mixSize);
      mixData[j] = INSTRUMENT_SAMPLES[i];
      mixCounter[j] = INSTRUMENT_SIZES[i];
    }
  }
}

static void setMasterVolume(int32_t* sample) {
  *sample = *sample * masterVolume >> 12;
}

static void writeBuffer(void) {
  uint32_t i, j;
  int32_t sample;
  for (i = 0; i < STEP_SIZE; ++i) {
    sample = 0;
    for (j = 0; j < mixSize; ++j) {
      if (mixCounter[j] == 0)
        continue;
      sample += *mixData[j]++;
      --mixCounter[j];
    }
    setMasterVolume(&sample);
    clamp12(&sample);
    AUDIO_BUFFER[i] = 2048 + sample;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Export
////////////////////////////////////////////////////////////////////////////////

int16_t STEP_SIZE = 480000 / 110;
int16_t AUDIO_BUFFER[8000] = { 0 };

void STEP_AUDIO(void) {
//  DMA_Cmd(DMA2_Channel3, DISABLE);
//  DMA_SetCurrDataCounter(DMA2_Channel3, STEP_SIZE);
//  DMA_Cmd(DMA2_Channel3, ENABLE);
  updateMix();
  writeBuffer();
}
