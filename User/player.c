#include "stm32f10x.h"

#include "drivers/audio.h"
#include "drivers/cd4051b.h"
#include "drivers/instrument.h"
#include "drivers/mastervolumepot.h"
#include "drivers/tempoencoder.h"

#include "app.h"
#include "pattern.h"
#include "player.h"

static int16_t Buffer[2][Player_BufferSize] = { 0 };
static int CurrBuffer = 0;
static int NextBuffer = 1;

static const int16_t* Data[Player_NumTracks] = { 0 };
static int16_t        Size[Player_NumTracks] = { 0 };
static int16_t        Index[Player_NumTracks] = { 0 };
static int16_t        Queue[Player_NumTracks] = { 0 };

static int head = 0;
static int tail = 0;

static int Period = 4000;

static void UpdatePeriod(void) {
  Period = 480000 / TempoEncoder_TempoBPM();
}

static void Push(int16_t item) {
  Queue[tail] = item;
  if (++tail == Player_NumTracks)
    tail = 0;
}

static int16_t Pop(void) {
  int16_t item = Queue[head];
  if (++head == Player_NumTracks)
    head = 0;
  return item;
}

static void ResetBuffer(void) {
  int i;
  for (i = 0; i < Player_BufferSize; ++i)
    Buffer[0][i] = Buffer[1][i] = 0x8000;
}

static void InitQueue(void) {
  int i;
  for (i = 0; i < Player_NumTracks; ++i)
    Push(i);
}

static void WriteBuffer(int16_t* buffer) {
  int track, volume, i;
  for (i = 0; i < Period; ++i)
    buffer[i] = 0;
  for (track = 0; track < Player_NumTracks; ++track) {
    volume = CD4051B_Read(Index[track]);
    for (i = 0; Size[track] && i < Period; ++i) {
      buffer[i] += *Data[track]++ * volume >> 12;
      if (--Size[track] == 0)
        Push(track);
    }
  }
  volume = MasterVolumePot_Read();
  for (i = 0; i < Period; ++i)
    buffer[i] = (buffer[i] * volume >> 12) ^ 0xFFFF8000;
}

static void EnqueueInstruments(void) {
  const uint16_t currPattern = App_CurrPattern();
  const uint16_t nextStep = App_NextStep();
  int i, j;
  for (i = 0; i < Instrument_NumInstruments; ++i) {
    if (Pattern_Data(currPattern)[i] & nextStep) {
      j = Pop();
      Data[j] = Instrument_Data(i);
      Size[j] = Instrument_Size(i);
      Index[j] = i;
    }
  }
}

static void RenderStep() {
  UpdatePeriod();
  EnqueueInstruments();
  WriteBuffer(Buffer[NextBuffer]);
}

extern void Player_Init(void) {
  static int init = 0;
  if (init) return;
  else init = 1;
  
  Audio_Init();

  ResetBuffer();
  InitQueue();
}

extern void Player_Start(void) {
  Audio_SetBuffer((uint32_t) Buffer[CurrBuffer], Period);
  RenderStep();
}

extern void Player_Stop(void) {
  Audio_SetBuffer(0, 0);
  ResetBuffer();
}

extern void DMA2_Channel3_IRQHandler(void) {
  if (DMA_GetITStatus(DMA2_IT_TC3)) {
    DMA_ClearITPendingBit(DMA2_IT_TC3);
    CurrBuffer ^= 0x1;
    NextBuffer ^= 0x1;
    Audio_SetBuffer((uint32_t) Buffer[CurrBuffer], Period);
    App_RotateCurrStep();
    RenderStep();
  }
}
