/*
 * inputs.h
 *
 *  Created on: Jan 10, 2019
 *      Author: jweinzierl
 */

#ifndef INPUTS_H_
#define INPUTS_H_

// System unique values
//----------------------------
// Solution tank levels
typedef enum
{
	IN_SOL_TANK_LEVEL_0,	//empty
	IN_SOL_TANK_LEVEL_1,	//1/4 full
	IN_SOL_TANK_LEVEL_2,	//1/2 full
	IN_SOL_TANK_LEVEL_3,	//3/4 full
	IN_SOL_TANK_LEVEL_4,	//full
	//
	IN_NUM_SOL_TANK_LEVELS

} in_sol_tank_level_t;

// Battery charge levels
// (don't change the order, keep them low-v to high-v)
typedef enum
{
	IN_BATTERY_LEVEL_LVC_2,		//low-voltage cutout 2
	IN_BATTERY_LEVEL_LVC_1,		//low-voltage cutout 1
	IN_BATTERY_LEVEL_0,
	IN_BATTERY_LEVEL_1,
	IN_BATTERY_LEVEL_2,
	IN_BATTERY_LEVEL_3,
	IN_BATTERY_LEVEL_4,
	IN_BATTERY_LEVEL_5,
	//
	IN_NUM_BATTERY_LEVELS

} in_battery_level_t;


//membrane button inputs
#define IN_BTN_SCRUB				(1u << 0)
#define IN_BTN_SCRUB_INC			(1u << 1)
#define IN_BTN_SCRUB_DEC			(1u << 2)
#define IN_BTN_SOLUTION				(1u << 3)
#define IN_BTN_SOLUTION_INC			(1u << 4)
#define IN_BTN_SOLUTION_DEC			(1u << 5)
#define IN_BTN_POWER				(1u << 6)
#define IN_BTN_VACUUM				(1u << 7)
#define IN_BTN_SPEED_LIMIT			(1u << 8)
#define IN_BTN_DETERGENT			(1u << 9)
#define IN_BTN_HEADLIGHT			(1u << 10)
#define IN_BTN_CLICK_ON				(1u << 11)
#define IN_BTN_DUST_GUARD			(1u << 12)
#define IN_BTN_MENU_UP				(1u << 13)
#define IN_BTN_MENU_RIGHT			(1u << 14)
#define IN_BTN_MENU_DOWN			(1u << 15)
#define IN_BTN_MENU_LEFT			(1u << 16)
#define IN_BTN_INFO					(1u << 17)
//
#define IN_NUM_BTNS					18
#define IN_BTN_ALL					(0x0003FFFFu)
#define IN_BTN_ALL_MENU				(IN_BTN_MENU_UP | IN_BTN_MENU_RIGHT | IN_BTN_MENU_DOWN | IN_BTN_MENU_LEFT | IN_BTN_INFO)
#define IN_BTN_ALL_FUNCTIONAL		(0x000001FBFu)

bool In_IsBtnPressed (uint32_t btnMask);
bool In_WasBtnPressed (uint32_t btnMask);

#endif /* INPUTS_H_ */
