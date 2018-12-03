#include "stm32f10x.h"

#include "drivers/audio.h"
#include "drivers/instrument.h"

#include "app.h"
#include "pattern.h"
#include "player.h"
#include "volume.h"

static uint16_t DoubleBuffer[Player_BufferSize << 1];
static uint16_t* Buffer1 = DoubleBuffer;
static uint16_t* Buffer2 = DoubleBuffer + Player_BufferSize;

static const int16_t* Data[Player_NumTracks] = { 0 };
static int16_t        Size[Player_NumTracks] = { 0 };
static int16_t        Index[Player_NumTracks] = { 0 };
static int16_t        Queue[Player_NumTracks] = { 0 };
static int32_t        Period = 4000 - 1;
static int32_t        Counter = 0;

static void Push(int16_t item) {
  static int tail = 0;
  Queue[tail] = item;
  if (++tail == Player_NumTracks)
    tail = 0;
}

static int16_t Pop(void) {
  static int head = 0;
  int16_t item = Queue[head];
  if (++head == Player_NumTracks)
    head = 0;
  return item;
}

static void InitQueue(void) {
  int i;
  for (i = 0; i < Player_NumTracks; ++i)
    Push(i);
}

static void InitIndex(void) {
  int i;
  for (i = 0; i < Player_NumTracks; ++i)
    Index[i] = -1;
}

static void ApplyVolume(int32_t* sample, int16_t volume) {
  *sample = *sample * volume >> 12;
}

static void ApplyClamp(int32_t* sample) {
  *sample = *sample > 32767 ? 32767 : *sample;
  *sample = *sample < -32768 ? -32768 : *sample;
}

static void ApplyBias(int32_t* sample) {
  *sample ^= 0xFFFF8000;
}

static int32_t ReadData(int i) {
  int32_t sample = *Data[i];
  int32_t volume = Volume_InstrumentVolume(Index[i]);
  ApplyVolume(&sample, volume);
  return sample;
}

static void WriteSample(uint16_t* buffer) {
  int32_t sample = 0;
  int i;
  for (i = 0; i < Player_NumTracks; ++i) {
    if (Index[i] == -1) {
      continue;
    } else if (Size[i] == 0) {
      Push(i);
      Index[i] = -1;
    } else {
      sample += ReadData(i);
      ++Data[i];
      --Size[i];
    }
  }
  ApplyVolume(&sample, Volume_MasterVolume());
  ApplyClamp(&sample);
  ApplyBias(&sample);
  *buffer = sample;
}

static void EnqueueInstruments(void) {
  uint16_t currPattern = App_CurrPattern();
  uint16_t currStep = App_CurrStep();
  int i, j;
  for (i = 0; i < Instrument_NumInstruments; ++i) {
    if (Pattern_Data(currPattern)[i] & currStep) {
      j = Pop();
      Data[j] = Instrument_Data(j);
      Size[j] = Instrument_Size(j);
      Index[j] = j;
    }
  }
}

static void WriteBuffer(uint16_t* buffer) {
  int i;
  for (i = 0; i < Player_BufferSize; ++i) {
    if (Counter > Period) {
      App_RotateCurrStep();
      EnqueueInstruments();
    }
    WriteSample(buffer + i);
    ++Counter;
  }
}

extern void Player_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;

  Audio_Init(DoubleBuffer, Player_BufferSize << 1);
  InitQueue();
  InitIndex();
}

extern void DMA2_Channel3_IRQHandler(void) {
  if (DMA_GetITStatus(DMA2_IT_HT3)) {
    WriteBuffer(Buffer1);
    DMA_ClearITPendingBit(DMA2_IT_HT3);
  } else if (DMA_GetITStatus(DMA2_IT_TC3)) {
    WriteBuffer(Buffer2);
    DMA_ClearITPendingBit(DMA2_IT_TC3);
  }
}
