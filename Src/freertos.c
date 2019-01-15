/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "stdbool.h"
#include "can.h"
#include "oled.h"
#include "display.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern bool		hb_led_state;

extern uint32_t	usart2_bytes_rcvd;
extern uint32_t	usart2_err_cnt;

extern uint32_t	can2_fifo0_msgs_rcvd;
extern uint32_t	can2_fifo1_msgs_rcvd;
extern uint32_t	can2_fifo0_err_cnt;
extern uint32_t	can2_fifo1_err_cnt;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId UartTaskHandle;
osThreadId CanTaskHandle;
osThreadId DisplayTaskHandle;
osMessageQId UartRxQueueHandle;
osMessageQId CanRxQueueHandle;
osMessageQId DisplayQueueHandle;
osMutexId uart2_tx_buffer_mutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void uart_task(void const * argument);
void can_task_run(void const * argument);
void display_task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* Create the mutex(es) */
  /* definition and creation of uart2_tx_buffer_mutex */
  osMutexDef(uart2_tx_buffer_mutex);
  uart2_tx_buffer_mutexHandle = osMutexCreate(osMutex(uart2_tx_buffer_mutex));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of UartTask */
  osThreadDef(UartTask, uart_task, osPriorityNormal, 0, 128);
  UartTaskHandle = osThreadCreate(osThread(UartTask), NULL);

  /* definition and creation of CanTask */
  osThreadDef(CanTask, can_task_run, osPriorityNormal, 0, 128);
  CanTaskHandle = osThreadCreate(osThread(CanTask), NULL);

  /* definition and creation of DisplayTask */
  osThreadDef(DisplayTask, display_task, osPriorityIdle, 0, 128);
  DisplayTaskHandle = osThreadCreate(osThread(DisplayTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of UartRxQueue */
  osMessageQDef(UartRxQueue, 64, uint8_t);
  UartRxQueueHandle = osMessageCreate(osMessageQ(UartRxQueue), NULL);

  /* definition and creation of CanRxQueue */
  osMessageQDef(CanRxQueue, 16, can_msg_t);
  CanRxQueueHandle = osMessageCreate(osMessageQ(CanRxQueue), NULL);

  /* definition and creation of DisplayQueue */
  osMessageQDef(DisplayQueue, 8, display_mode_t);
  DisplayQueueHandle = osMessageCreate(osMessageQ(DisplayQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
	uint32_t 		u32_cnt = 0;
	uint8_t 		nr_frame = 1;
	display_mode_t	disp_mode;
	uint8_t 	nr_modes_sent = 0;
	for(;;)
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
		if ((u32_cnt % 25) == 0)
		{
			switch(nr_frame)
			{
				case 1:
					//	oled_clear_screen (true);
					nr_frame = 2;
					disp_mode = DISPLAY_MODE_PURGE;
					break;
				case 2:
					//	oled_fill_screen(true);
					nr_frame = 3;
					disp_mode = DISPLAY_MODE_CHARGING;
					break;
				case 3:
					nr_frame = 4;
					//	oled_draw_test_pattern_1(true);
					disp_mode = DISPLAY_MODE_ESTOP;
					break;
				case 4:
					nr_frame = 5;
					//	oled_draw_test_pattern_2(true);
					disp_mode = DISPLAY_MODE_RTF;
					break;
				case 5:
					nr_frame = 1;
					//	oled_draw_test_pattern_2(true);
					disp_mode = DISPLAY_MODE_RTF;
					break;
				default:
					nr_frame = 1;
			};

			xQueueSendToBack(DisplayQueueHandle, &disp_mode, 1000);
			xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
			printf ("main loop run(%ld), DISPLAY_MODE %d sent\n", u32_cnt, disp_mode);
			xSemaphoreGive(uart2_tx_buffer_mutexHandle);
		}

		xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
		printf ("main loop run(%ld), U2_Rx: %ld, U2_Err: %ld, C0_Rx: %ld, C1_Rx: %ld, C_Err: %ld\n", ++u32_cnt, usart2_bytes_rcvd, usart2_err_cnt, can2_fifo0_msgs_rcvd, can2_fifo1_msgs_rcvd, (can2_fifo0_err_cnt+can2_fifo1_err_cnt));
		xSemaphoreGive(uart2_tx_buffer_mutexHandle);

		osDelay(500);
	}
}

/* USER CODE BEGIN Header_uart_task */
/**
* @brief Function implementing the UartTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_uart_task */
void uart_task(void const * argument)
{
  /* USER CODE BEGIN uart_task */
	uint8_t queue_rx_data;
	/* Infinite loop */
	for(;;)
	{
		if (xQueueReceive(UartRxQueueHandle, &queue_rx_data, 5000) == pdPASS)
		{
			xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
			printf ("-%c-\n", queue_rx_data);
			xSemaphoreGive(uart2_tx_buffer_mutexHandle);
		}
		else
		{
			xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
			printf ("-No Rx data-\n");
			xSemaphoreGive(uart2_tx_buffer_mutexHandle);
		}
	}

  /* USER CODE END uart_task */
}

/* USER CODE BEGIN Header_can_task_run */
/**
* @brief Function implementing the CanTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_can_task_run */
void can_task_run(void const * argument)
{
  /* USER CODE BEGIN can_task_run */
  /* Infinite loop */
	HAL_StatusTypeDef 	hal_sts;
	uint32_t 			u32_cnt = 0;
	can_msg_t			rx_msg;
	uint16_t			n;
	for(;;)
	{
		if (xQueueReceive(CanRxQueueHandle, &rx_msg, 100) == pdPASS)
		{
			xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
			printf ("-CAN-Rx(), DLC=%d,", rx_msg.dlc);
			for (n=0; n<rx_msg.dlc; n++)
			{
				printf (" %02X", rx_msg.data.byte[n]);
			}
			printf ("-\n");
			xSemaphoreGive(uart2_tx_buffer_mutexHandle);
		}
		else
		{
			if ((++u32_cnt % 10) == 0)
			{
				hal_sts = CAN2_SendPDO1 (u32_cnt);
				if (hal_sts != HAL_OK)
				{
					xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
					printf("CAN bad transmit: %ld\n", u32_cnt);
					xSemaphoreGive(uart2_tx_buffer_mutexHandle);
				}
//		  	  	else
//		  	  	{
//			    	xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
//			    	printf("CAN good transmit: %ld\n", u32_cnt);
//			    	xSemaphoreGive(uart2_tx_buffer_mutexHandle);
//		  	  	}
		  }
	  }
  }
  /* USER CODE END can_task_run */
}

/* USER CODE BEGIN Header_display_task */
/**
* @brief Function implementing the DisplayTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_display_task */
void display_task(void const * argument)
{
  /* USER CODE BEGIN display_task */
	run_display_task (argument);
  /* USER CODE END display_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
