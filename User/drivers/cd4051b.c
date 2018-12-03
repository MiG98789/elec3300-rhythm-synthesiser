#include "stm32f10x.h"

#include "cd4051b.h"
#include "delay.h"

static const uint8_t    Map[] = { 2, 6, 5, 4, 7, 0, 3, 1 };
static int16_t          State[CD4051B_NumChannels];
static int              CurrChannel;

static void InitState(void) {
  int i;
  for (i = 0; i < CD4051B_NumChannels; ++i)
    State[i] = CD4051B_MaxValue;
}

static void InitGPIO(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  
  RCC_APB2PeriphClockCmd(CD4051B_COM_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(CD4051B_A_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(CD4051B_B_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(CD4051B_C_CLK, ENABLE);
  
  GPIO_InitStruct.GPIO_Pin = CD4051B_A_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CD4051B_A_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = CD4051B_B_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CD4051B_B_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = CD4051B_C_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CD4051B_C_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Pin = CD4051B_COM_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CD4051B_COM_PORT, &GPIO_InitStruct);
}

static void InitADC(void) {
  ADC_InitTypeDef ADC_InitStruct;
  
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
  RCC_APB2PeriphClockCmd(CD4051B_ADC_CLK, ENABLE);
  
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_Init(CD4051B_ADC, &ADC_InitStruct);
	
	ADC_RegularChannelConfig(CD4051B_ADC, CD4051B_ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	ADC_Cmd(CD4051B_ADC, ENABLE);
  
	ADC_ResetCalibration(CD4051B_ADC);
	while (ADC_GetResetCalibrationStatus(CD4051B_ADC));
	ADC_StartCalibration(CD4051B_ADC);
	while (ADC_GetCalibrationStatus(CD4051B_ADC));
}

extern void CD4051B_Init() {
  static int init = 0;
  if (init) return;
  else init = 1;
  
  Delay_Init();

  CurrChannel = 0;
  InitState();
  InitGPIO();
  InitADC();
}

extern void CD4051B_Poll(void) {
  BitAction A = CurrChannel & 0x1 ? Bit_SET : Bit_RESET;
  BitAction B = CurrChannel & 0x2 ? Bit_SET : Bit_RESET;
  BitAction C = CurrChannel & 0x4 ? Bit_SET : Bit_RESET;
  GPIO_WriteBit(CD4051B_A_PORT, CD4051B_A_PIN, A);
  GPIO_WriteBit(CD4051B_B_PORT, CD4051B_B_PIN, B);
  GPIO_WriteBit(CD4051B_C_PORT, CD4051B_C_PIN, C);
  Delay_Cycles(52); // 722 ns
  ADC_SoftwareStartConvCmd(CD4051B_ADC, ENABLE);
  while (ADC_GetSoftwareStartConvStatus(CD4051B_ADC));
  State[Map[CurrChannel]] = ADC_GetConversionValue(CD4051B_ADC);
  CurrChannel = (CurrChannel + 1) & 0x7;
}

int16_t CD4051B_Read(int i) {
  return State[i];
}
