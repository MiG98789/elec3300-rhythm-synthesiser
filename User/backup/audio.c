#include "audio.h"
#include "instruments.h"
#include "pattern.h"
<<<<<<< Updated upstream
#include "volume.h"
=======
#include "lcd.h"
>>>>>>> Stashed changes

#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"

////////////////////////////////////////////////////////////////////////////////
// Static
////////////////////////////////////////////////////////////////////////////////

static const uint32_t bufferSize = 0x1000;
static int16_t        doubleBuffer[bufferSize << 1];

<<<<<<< Updated upstream
static uint32_t min_element(const uint16_t* const first, const uint16_t* const last) {
  const uint16_t* smallest = first;
  const uint16_t* it = first;
  while (++it != last)
    if (*it < *smallest)
      smallest = it;
  return smallest - first;
=======
static const uint8_t  numTracks = 8;
static const int16_t* data[numTracks] = { 0 };
static int16_t        count[numTracks] = { 0 };
static uint8_t        queue[numTracks] = { 0 };
static uint8_t        read = 0;
static uint8_t        write = 0;
static uint32_t       reload = 4000 - 1;
static uint32_t       counter;

static void setMasterVolume(int32_t* sample) {
  static const int16_t masterVolume = 4096;
  *sample = *sample * masterVolume >> 12;
>>>>>>> Stashed changes
}

static void clamp(int32_t* sample) {
  *sample = *sample > 2047 ? 2047 : *sample;
  *sample = *sample < -2048 ? -2048 : *sample;
}

static void bias(int32_t* sample) {
  *sample ^= 
}

static void updateTracks(void) {
  uint32_t i;
  for (i = 0; i < INSTRUMENT_COUNT; ++i) {
    if (PATTERNS[CURR_PATTERN][i] & CURR_STEP) {
      data[queue[read]] = INSTRUMENT_SAMPLES[i];
      count[queue[read]] = INSTRUMENT_SIZES[i];
      read = (read + 1) & (numTracks - 1);
    }
  }
}

<<<<<<< Updated upstream
static void setMasterVolume(int32_t* sample) {
  *sample = *sample * MASTER_VOLUME >> 12;
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
=======
static void writeBuffer(uint32_t pos) {
  uint32_t i;
  int32_t sample = 0;
  for (i = 0; i < numTracks; ++i) {
    if (count[i] == -1) {
      continue;
    } else if (count[i] == 0) {
      queue[write] = i;
      write = (write + 1) & (numTracks - 1);
    } else {
      sample += *data[i]++;
>>>>>>> Stashed changes
    }
    --count[i];
  }
  setMasterVolume(&sample);
  clamp(&sample);
  doubleBuffer[pos] = 2048 + sample;
}

static void hardwareInit(void) {
  DMA_InitTypeDef DMA_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
  DAC_InitTypeDef DAC_InitStruct;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &DAC->DHR12R1;
  DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t) doubleBuffer;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStruct.DMA_BufferSize = bufferSize << 1;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;
  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel3, &DMA_InitStruct);
  DMA_ITConfig(DMA2_Channel3, DMA_IT_HT | DMA_IT_TC, ENABLE);
  DMA_Cmd(DMA2_Channel3, ENABLE);
  
  NVIC_InitStruct.NVIC_IRQChannel = DMA2_Channel3_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
  
  DAC_InitStruct.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStruct);
  DAC_DMACmd(DAC_Channel_1, ENABLE);
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_SetChannel1Data(DAC_Align_12b_R, 2048);
}  

////////////////////////////////////////////////////////////////////////////////
// Export
////////////////////////////////////////////////////////////////////////////////

void AUDIO_INIT(void) {
  uint32_t i;
  hardwareInit();
  counter = reload;
  for (i = 0; i < 8; ++i)
    queue[i] = i;
}

void AUDIO_TICK(void) {
  uint32_t offset = DMA_GetITStatus(DMA2_IT_HT3) ? bufferSize : 0;
  uint32_t pos;
  for (pos = 0; pos < bufferSize; ++pos) {
    if (counter == reload) {
      PATTERN_TICK();
      updateTracks();
      counter = 0;
    }
    writeBuffer(pos);
    ++counter;
  }
}
