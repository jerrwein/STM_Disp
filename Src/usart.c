/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "stdbool.h"
//#include "stdlib.h"
//#include "stdio.h"
#include "string.h"

#define DMA_BUF_SZ 100

extern osMutexId uart2_tx_buffer_mutexHandle;
extern osMessageQId UartRxQueueHandle;

extern bool	hb_led_state;

uint8_t 				DMA_TxBuffer[DMA_BUF_SZ];
extern __IO uint8_t 	dma_transmit_complete;
uint32_t				usart2_bytes_rcvd = 0;
uint32_t				usart2_err_cnt = 0;

int _write(int32_t file, uint8_t *ptr, int32_t len);

int _write(int32_t file, uint8_t *ptr, int32_t len)
{
	// Wait for any previous transmission to complete (or better yet, queue incoming ones)
	while (dma_transmit_complete != 1)
	{
		if (!hb_led_state)
		{
			LL_GPIO_SetOutputPin(HB_LED_GPIO_Port, HB_LED_Pin);
			hb_led_state = true;
		}
		else
		{
			LL_GPIO_ResetOutputPin(HB_LED_GPIO_Port, HB_LED_Pin);
			hb_led_state = false;
		}
		LL_mDelay(50);
	};

	if (len < DMA_BUF_SZ)
	{
		memcpy (DMA_TxBuffer, ptr, len);
		LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_6, len);

		/* Enable DMA Channel Tx */
		dma_transmit_complete = 0;
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_6);
		return len;
	}
	else
	{
		return 0;
	}
}

void USART_CharReception_Callback(void)
{
	uint8_t char_rcvd;
	BaseType_t HigherPriorityTaskWasWoken = pdFALSE;

	/* Read Received character. RXNE flag is cleared by reading of DR register */
	char_rcvd = LL_USART_ReceiveData8(USART2);
	usart2_bytes_rcvd++;

	xQueueSendToBackFromISR(UartRxQueueHandle, &char_rcvd, &HigherPriorityTaskWasWoken);

	if (HigherPriorityTaskWasWoken)
	{
		taskYIELD();
	}

	/* Check if received value is corresponding to specific one : S or s */
//	if ((0x30 <= char_rcvd) && (char_rcvd <= 0x7a))
//	{
//		xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
//		printf ("\n-%c\n-", char_rcvd);
//		xSemaphoreGive(uart2_tx_buffer_mutexHandle);
//	}
	/* Echo received character on TX */
	//LL_USART_TransmitData8(USARTx_INSTANCE, received_char);
}

void USART_Error_Callback(void)
{
	__IO uint32_t sr_reg;

	usart2_err_cnt++;

	/* Disable USARTx_IRQn */
	NVIC_DisableIRQ(USART2_IRQn);

	/* Error handling example :
	- Read USART SR register to identify flag that leads to IT raising
	- Perform corresponding error handling treatment according to flag
	*/
	sr_reg = LL_USART_ReadReg(USART2, SR);
	if (sr_reg & LL_USART_SR_NE)
	{
		/* case Noise Error flag is raised : ... */
	    //LED_Blinking(LED_BLINK_FAST);
	}
	else
	{
		/* Unexpected IT source : Set LED to Blinking mode to indicate error occurs */
	    //LED_Blinking(LED_BLINK_ERROR);
	}
}

void USART_TransferError_Callback(void)
{
	// TBD;
}
/* USER CODE END 0 */

/* USART2 init function */

void MX_USART2_UART_Init(void)
{
  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
  
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**USART2 GPIO Configuration  
  PA2   ------> USART2_TX
  PA3   ------> USART2_RX 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART2 DMA Init */
  
  /* USART2_TX Init */
  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_6, LL_DMA_CHANNEL_4);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_6, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_6, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_STREAM_6, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_6, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_6, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_6, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_6, LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_6);

  /* USART2 interrupt Init */
  NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(USART2_IRQn);

  USART_InitStruct.BaudRate = 38400;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART2, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART2);
  LL_USART_Enable(USART2);

}

/* USER CODE BEGIN 1 */
void USART_DMA_Init(void)
{
	LL_DMA_ConfigTransfer(DMA1, LL_DMA_STREAM_6,
                                LL_DMA_DIRECTION_MEMORY_TO_PERIPH |
                                LL_DMA_PRIORITY_HIGH              |
                                LL_DMA_MODE_NORMAL                |
                                LL_DMA_PERIPH_NOINCREMENT         |
                                LL_DMA_MEMORY_INCREMENT           |
                                LL_DMA_PDATAALIGN_BYTE            |
                                LL_DMA_MDATAALIGN_BYTE);
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_6,
          					     (uint32_t)DMA_TxBuffer,
                                 LL_USART_DMA_GetRegAddr(USART2),
                                 LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_6));

	/* Disable DMA1 Tx Channel */
	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_6);

	/* Enable DMA Channel Interrupt */
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_6);

	/* Enable DMA TX Interrupt */
	LL_USART_EnableDMAReq_TX(USART2);

	LL_USART_EnableIT_RXNE(USART2);
	// LL_USART_EnableIT_ERROR(USART2);
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
