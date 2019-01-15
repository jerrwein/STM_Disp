/*
 * system.h
 *
 *  Created on: Jan 10, 2019
 *      Author: jweinzierl
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

uint32_t 	Sys_GetTick(void);
bool 		Sys_TimerExpired (uint32_t startTick, uint32_t msTimespan);

#endif /* SYSTEM_H_ */
