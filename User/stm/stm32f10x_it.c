/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f10x_it.h"
#include "stm32f10x_exti.h"
#include "lcd.h"
#include "helper.h"
#include "pattern.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

void CLK_Pulse(int duration) {
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, HIGH);
		Delayus(duration);
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, LOW);
}

/**
  * @brief  This handles EXTI_0 IRQ 
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void) {
	uint8_t buttonRawIndex = 0;
	uint8_t buttonMapIndex = 0;
	uint8_t inputDataBit = 0;
	uint16_t result = 0;
	char output[17];
	
	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {				
		// SH/LD: PB7
		// CLK:   PB5
		// QH:    PB6
		
		// Set CLK to low
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, LOW);
		
		// 2) Set SH/LD to low
		GPIO_WriteBit(GPIOB, GPIO_Pin_7, LOW);
		
		// 3) Set CLK to high, delayus, CLK to low
		CLK_Pulse(10000);
		
		// 4) Set SH/LD to high
		GPIO_WriteBit(GPIOB, GPIO_Pin_7, HIGH);
		
		// 5) 16-for loop, each time do step 3 and read QH
		for (buttonRawIndex = 0; buttonRawIndex < 16; buttonRawIndex++) {
			inputDataBit = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
			
			result = result << 1;
			result |= inputDataBit;
			
			buttonMapIndex = PATTERN_BUTTON_MAPPING[buttonRawIndex];
			output[buttonMapIndex] = inputDataBit + '0';

			CLK_Pulse(10000);
		}
		output[16] = '\0';
		LCD_DrawString(10, 10, output);

	/* Clear the Key Button EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
