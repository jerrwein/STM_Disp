/*
 * inputs.c
 *
 *  Created on: Jan 12, 2019
 *      Author: jerry
 */

#include "stdint.h"
#include "stdbool.h"

#include "inputs.h"


static uint32_t m_btnStates;
static uint32_t m_btnPressed;				//pressed-to-released state change
//static uint32_t m_btnReleased;				//released-to-pressed state change


/******************************************************************************
* In_IsBtnPressed
*	Return membrane button's status.
******************************************************************************/

inline bool In_IsBtnPressed (uint32_t btnMask)
{
	return (((m_btnStates & btnMask) == btnMask) ? true : false);
}

/******************************************************************************
* In_WasBtnPressed
*	Return membrane button's released-to-pressed change of state status.
*
*	This function only returns true once for each change of state event.
*	Once it has returned true, it will return false until the next
*	released-to-pressed state change.
******************************************************************************/
bool In_WasBtnPressed (uint32_t btnMask)
{
	bool pressed = false;
	if ((m_btnPressed & btnMask) == btnMask)
	{
		m_btnPressed &= ~btnMask;
		pressed = true;
	}
	return (pressed);
}


