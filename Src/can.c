/**
  ******************************************************************************
  * File Name          : CAN.c
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
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
#include "can.h"

/* USER CODE BEGIN 0 */
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

extern osMutexId uart2_tx_buffer_mutexHandle;
extern osMessageQId CanRxQueueHandle;

uint32_t	can2_fifo0_msgs_rcvd = 0;
uint32_t	can2_fifo1_msgs_rcvd = 0;
uint32_t	can2_fifo0_err_cnt = 0;
uint32_t	can2_fifo1_err_cnt = 0;

CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;

  /* The initialization values shown below in MX_CAN2_Init() will yield a bit rate of 250k
   * hcan2.Init.Prescaler = 10;
   * hcan2.Init.Mode = CAN_MODE_NORMAL;
   * hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
   * hcan2.Init.TimeSeg1 = CAN_BS1_16TQ;
   * hcan2.Init.TimeSeg2 = CAN_BS2_3TQ;
   */

/* USER CODE END 0 */

CAN_HandleTypeDef hcan2;

/* CAN2 init function */
void MX_CAN2_Init(void)
{

  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 10;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_16TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_3TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = ENABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspInit 0 */

  /* USER CODE END CAN2_MspInit 0 */
    /* CAN2 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();
    __HAL_RCC_CAN2_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN2 GPIO Configuration    
    PB13     ------> CAN2_TX
    PB5     ------> CAN2_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* CAN2 interrupt Init */
    HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
  /* USER CODE BEGIN CAN2_MspInit 1 */

  /* USER CODE END CAN2_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspDeInit 0 */

  /* USER CODE END CAN2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();
    __HAL_RCC_CAN2_CLK_DISABLE();
  
    /**CAN2 GPIO Configuration    
    PB13     ------> CAN2_TX
    PB5     ------> CAN2_RX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13|GPIO_PIN_5);

    /* CAN2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_RX1_IRQn);
  /* USER CODE BEGIN CAN2_MspDeInit 1 */

  /* USER CODE END CAN2_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void CAN2_ConfigFilters(void)
{
	CAN_FilterTypeDef  sFilterConfig;

	/*# Configure the CAN Filters */
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
//	sFilterConfig.FilterIdHigh = 0x0000;
	/* As long as ID contains something non-zero, an empty mask works! */
	// sFilterConfig.FilterIdHigh = (0x7ff << 5);
	sFilterConfig.FilterIdHigh = (0x111 << 5);
	sFilterConfig.FilterIdLow = 0x0000;
//	sFilterConfig.FilterMaskIdHigh = (0x100 <<5);
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 0;	//14
	if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK)
	{
		/* Filter configuration Error */
		xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
		printf ("CAN2_ConfigFilters() failed to configure filters\n");
		xSemaphoreGive(uart2_tx_buffer_mutexHandle);
		Error_Handler();
	}
}

void CAN2_Start(void)
{
	if (HAL_CAN_Start(&hcan2) != HAL_OK)
	{
		/* Start Error */
		xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
		printf ("CAN2_Start() failed to start\n");
		xSemaphoreGive(uart2_tx_buffer_mutexHandle);
		Error_Handler();
	}
}

void CAN2_ActivateNotification(void)
{
	/* Activate CAN RX notification */
	if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		/* Notification Error */
		xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
		printf ("CAN2_ActivateNotification() failed to activate FIFO-0 MSG pending\n");
		xSemaphoreGive(uart2_tx_buffer_mutexHandle);
		Error_Handler();
	}
	if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
	{
		/* Notification Error */
		xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
		printf ("CAN2_ActivateNotification() failed to activate FIFO-1 MSG pending\n");
		xSemaphoreGive(uart2_tx_buffer_mutexHandle);
		Error_Handler();
	}
}

/* Override weak callback */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	can_msg_t	rx_msg;
	BaseType_t HigherPriorityTaskWasWoken = pdFALSE;
	uint16_t n;

	can2_fifo0_msgs_rcvd++;

	/* Get RX message */
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
	{
		/* Reception Error */
		can2_fifo0_err_cnt++;
		Error_Handler();
	}
	else
	{
		rx_msg.id = (uint16_t)RxHeader.StdId;
		rx_msg.dlc = (uint16_t)RxHeader.DLC;
		for (n=0; n<rx_msg.dlc; n++)
		{
			rx_msg.data.byte[n] = RxData[n];
		}

		xQueueSendToBackFromISR(CanRxQueueHandle, &rx_msg, &HigherPriorityTaskWasWoken);

		if (HigherPriorityTaskWasWoken)
		{
			taskYIELD();
		}
	}


//	printf ("HCAN_Callback(0): ->\n");
//	printf ("HCAN_Callback(0): ID:%03X, DLC: %d\n", RxHeader.StdId, RxHeader.DLC);
//	if ((RxHeader.StdId == 0x123) && (RxHeader.IDE == CAN_ID_STD) && (RxHeader.DLC == 4))
//	{
//		xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
//		printf ("HCAN_Callback(0): ID:%03lX, B[0]: %02X, B[1]: %02X, B[2]: %02X, B[3]: %02X\n", RxHeader.StdId, RxData[0], RxData[1], RxData[2], RxData[3]);
//		xSemaphoreGive(uart2_tx_buffer_mutexHandle);
//	}
}

/* Override weak callback */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	can_msg_t	rx_msg;
	BaseType_t HigherPriorityTaskWasWoken = pdFALSE;
	uint16_t n;

	can2_fifo1_msgs_rcvd++;

	/* Get RX message */
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, RxData) != HAL_OK)
	{
		/* Reception Error */
		can2_fifo1_err_cnt++;
		Error_Handler();
	}
	else
	{
		rx_msg.id = (uint16_t)RxHeader.StdId;
		rx_msg.dlc = (uint16_t)RxHeader.DLC;
		for (n=0; n<rx_msg.dlc; n++)
		{
			rx_msg.data.byte[n] = RxData[n];
		}

		xQueueSendToBackFromISR(CanRxQueueHandle, &rx_msg, &HigherPriorityTaskWasWoken);

		if (HigherPriorityTaskWasWoken)
		{
			taskYIELD();
		}
	}
}

uint32_t CAN2_SendPDO1(uint32_t can_cnt)
{
	HAL_StatusTypeDef hal_sts;

	TxHeader.StdId = 0x135;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 4;

	TxData[0] = (uint8_t)(can_cnt & 0x000000ff);
	TxData[1] = (uint8_t)((can_cnt>>8) & 0x000000ff);
	TxData[2] = (uint8_t)((can_cnt>>16) & 0x000000ff);
	TxData[3] = (uint8_t)((can_cnt>>24) & 0x000000ff);
	TxData[4] = (uint8_t)(0x00);
	TxData[5] = (uint8_t)(0x00);
	TxData[6] = (uint8_t)(0x00);
	TxData[7] = (uint8_t)(0x00);

	hal_sts = HAL_CAN_AddTxMessage(&hcan2, &TxHeader, TxData, &TxMailbox);
	return (uint32_t)hal_sts;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
