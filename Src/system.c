/*
 * system.c
 *
 *  Created on: Jan 10, 2019
 *      Author: jweinzierl
 */

#include "stdint.h"
#include "stdbool.h"
#include "system.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"

/******************************************************************************
* Sys_GetTick
*	Return current system tick count.
*	Wrapper for rtos get tick function.
******************************************************************************/
inline uint32_t Sys_GetTick(void)
{
	return (xTaskGetTickCount());
}

/******************************************************************************
* Sys_TimerExpired
*	Return whether or not msTimespan has elapsed since startTick.
******************************************************************************/
bool Sys_TimerExpired (uint32_t startTick, uint32_t msTimespan)
{
	return (((xTaskGetTickCount() - startTick) / portTICK_RATE_MS) >= msTimespan);
}
