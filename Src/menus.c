/******************************************************************************
* menus.c
*
* Application specific implementation for menu operation.
*
* Copyright (c) 2014 Nilfisk-Advance, Inc. All rights reserved.
*
* Version History
*------------------------------------------------------------------------------
* 0.01, 15Oct2014, CParker
*	- file created
*
******************************************************************************/
#include "stdint.h"
#include "stdbool.h"

#include "system.h"
#include "menus.h"
#include "display.h"
#include "oled.h"
#include "icons.h"
//#include "text.h"
//#include "modes.h"
#include "inputs.h"
//#include "outputs.h"
//#include "leds.h"
//#include "bootloader.h"
//#include "parameters.h"
//#include "debug.h"
//#include <string.h>


/*
*	Defines
*******************************************************************************/

/*
*	Menu Info Table
*******************************************************************************/
typedef struct  
{
	menu_page_id_t pageId;
	void (*menuLoad) (menu_page_t*, uint32_t);
	void (*menuRun) (menu_page_t*);
	 
} menu_info_t;

static const menu_info_t mk_menuInfo[MENU_NUM_MENUS] =
{
	//level 1 menu
	[MENU_ID_TOP]				= { MENU_PAGE_1	,TopMenu_Load					,TopMenu_Run }
	//level 2 menus
//	[MENU_ID_CONFIG]			= { MENU_PAGE_2	,ConfigMenu_Load				,ConfigMenu_Run },
//	[MENU_ID_OPTIONS]			= { MENU_PAGE_2	,OptionsMenu_Load				,OptionsMenu_Run },
//	[MENU_ID_SERVICE]			= { MENU_PAGE_2	,ServiceMenu_Load				,ServiceMenu_Run },
//	[MENU_ID_HOURS]				= { MENU_PAGE_2	,HoursMenu_Load					,HoursMenu_Run },
//	[MENU_ID_FAULTS]			= { MENU_PAGE_2	,FaultsMenu_Load				,FaultsMenu_Run },
//	[MENU_ID_KEYS]				= { MENU_PAGE_2	,KeysMenu_Load					,KeysMenu_Run },
//	[MENU_ID_SYSTEM]			= { MENU_PAGE_2	,SystemMenu_Load				,SystemMenu_Run },
	//level 3 menus
//	[MENU_ID_E2_STATUS]			= { MENU_PAGE_3	,ServiceMenu_LoadE2Status		,ServiceMenu_RunE2Status },
//	[MENU_ID_E3_STATUS]			= { MENU_PAGE_3	,ServiceMenu_LoadE3Status		,ServiceMenu_RunE3Status },
//	[MENU_ID_E4_STATUS]			= { MENU_PAGE_3	,ServiceMenu_LoadE4Status		,ServiceMenu_RunE4Status },
//	[MENU_ID_OUTPUT_TEST]		= { MENU_PAGE_3	,ServiceMenu_LoadOutputTest		,ServiceMenu_RunOutputTest },
//	[MENU_ID_PANEL_TEST]		= { MENU_PAGE_3	,ServiceMenu_LoadPanelTest		,ServiceMenu_RunPanelTest },
//	[MENU_ID_ACTIVE_FAULTS]		= { MENU_PAGE_3	,FaultsMenu_LoadActiveFaults	,FaultsMenu_RunActiveFaults },
//	[MENU_ID_FAULT_HISTORY]		= { MENU_PAGE_3	,FaultsMenu_LoadFaultHistory	,FaultsMenu_RunFaultHistory },
//	[MENU_ID_READ_KEY]			= { MENU_PAGE_3	,KeysMenu_LoadReadKey			,KeysMenu_RunReadKey },
//	[MENU_ID_KEY_LIST]			= { MENU_PAGE_3	,KeysMenu_LoadKeyList			,KeysMenu_RunKeyList },
//	[MENU_ID_INSTALL_FIRMWARE]	= { MENU_PAGE_3	,SystemMenu_LoadInstallFirmware	,SystemMenu_RunInstallFirmware },
//	[MENU_ID_IMPACT_LOG]		= { MENU_PAGE_3	,SystemMenu_LoadImpactLog		,SystemMenu_RunImpactLog },
};


/*
*	Variables
*******************************************************************************/
static menu_id_t m_menuId;
static menu_id_t m_newMenuId;
static uint32_t  m_startingItem;

static menu_page_t m_page[MENU_NUM_PAGES];

static bool m_headlightState;

/*
*	Globals
*******************************************************************************/
//global buffers for use by all menus (only 1 menu can be shown at a time so
//there should be no conflict)
char g_menuLabelBfr[32];
char g_menuValueBfr[32];

/*
*	Private Function Prototypes
*******************************************************************************/
//none

/******************************************************************************
* Menu_Run
*	Run the menu state machine.
******************************************************************************/
void Menu_Run (void)
{
	const menu_info_t *menu = &mk_menuInfo[m_newMenuId];
	menu_page_t *page = &m_page[menu->pageId];
	
	//check if we need to change menus
	if (m_newMenuId != m_menuId)
	{
		Menu_Reset(false);
		
		/*
		* if going to a higher level menu, reset the lower level one so that
		* it will reload when called again
		* in some cases, when you go from low level to high level menu and
		* then back to same low level menu without calling any other low level
		* menus, the original low level menu will start where it left off
		* instead of resetting
		*/
		menu_page_id_t oldPage = mk_menuInfo[m_menuId].pageId;
		if (menu->pageId < oldPage)
		{
			m_page[oldPage].menuId = MENU_NUM_MENUS;
		}
	
		//call the menu's load function
		//- set it to the designated starting item if necessary
		menu->menuLoad(page, m_startingItem);
		
		page->drawScreen = true;
		page->menuId = m_newMenuId;
		m_menuId = m_newMenuId;
	}
	
	//call the menu run function...
	menu->menuRun(page);
}

/******************************************************************************
* Menu_SetMenu
*	Set the active menu.
******************************************************************************/
void Menu_SetMenu (menu_id_t menuId, uint32_t startingItem)
{
// jmw	assert(menuId < MENU_NUM_MENUS);
	m_newMenuId = menuId;
	m_startingItem = startingItem;
	
	//if going to output or panel test, remember the state of the headlight
	//so can set it back when leaving the test
	if ((menuId == MENU_ID_OUTPUT_TEST) || (menuId == MENU_ID_PANEL_TEST))
	{
//		m_headlightState = Out_IsDoutOn(OUT_DOUT_HEADLIGHT);
	}
}

/******************************************************************************
* Menu_GetMenu
*	Get the active menu.
******************************************************************************/
inline menu_id_t Menu_GetMenu (void)
{
	return (m_menuId);
}

/******************************************************************************
* Menu_GetCurrentItem
*	Return the currently active menu item.
******************************************************************************/
inline uint32_t Menu_GetCurrentItem (void)
{
	return ((uint32_t)(m_page[mk_menuInfo[m_menuId].pageId].current));
}

/******************************************************************************
* Menu_IsActive
*	Return whether or not menu mode is active.
******************************************************************************/
/*
inline bool Menu_IsActive (void)
{
	return ((bool)(Display_GetState() == DISPLAY_STATE_MENU));
}
*/

/******************************************************************************
* Menu_Reset
*	Reset menu page parameters.
*	Reset any systems that may have been altered during the menu run.
******************************************************************************/
void Menu_Reset (bool clearPages)
{
	//if leaving output test, turn off all outputs
	if (m_menuId == MENU_ID_OUTPUT_TEST)
	{
	// jmw	Sys_StopFunctionalSystems();

		//re-enable certain outputs in case they were disabled
	// jmw	Sys_EnableControlOutputs();
	}
	//if leaving panel test, turn off leds
	else if (m_menuId == MENU_ID_PANEL_TEST)
	{
// jmw		Led_Off(LED_ALL);

		//re-enable headlight control
// jmw		if (g_paramConfig.flags & PARAM_CFG_FLAG_HEADLIGHT_INSTALLED)
		{
// jmw			Out_EnableAutoControl(OUT_CTRL_HEADLIGHT, true);
		}
	}
	//if leaving firmware update, ensure file is closed
	else if (m_menuId == MENU_ID_INSTALL_FIRMWARE)
	{
// jmw		Boot_CloseFile();
	}
	
	//set menuId to unused menu so next call to load will force a reload
	//other parameters will be reset when menu is reloaded
	if (clearPages)
	{
		m_menuId = MENU_NUM_MENUS;
		for (uint32_t i = 0; i < MENU_NUM_PAGES; i++)
		{
			m_page[i].menuId = MENU_NUM_MENUS;
		}
	}
}

/******************************************************************************
* Menu_DownBtnPressedOrHeld
*	Check if menu down button was pressed or is being held.
******************************************************************************/
bool Menu_DownBtnPressedOrHeld (void)
{
	static uint32_t btnTimer;
	bool isPressed = false;
	if (In_WasBtnPressed(IN_BTN_MENU_DOWN) ||
		(In_IsBtnPressed(IN_BTN_MENU_DOWN) && Sys_TimerExpired(btnTimer, MENU_FAST_UPDATE_RATE_MS)))
	{
		isPressed = true;
		btnTimer = Sys_GetTick();
	}
	return (isPressed);
}

/******************************************************************************
* Menu_UpBtnPressedOrHeld
*	Check if menu up button was pressed or is being held.
******************************************************************************/
bool Menu_UpBtnPressedOrHeld (void)
{
	static uint32_t btnTimer;
	bool isPressed = false;
	if (In_WasBtnPressed(IN_BTN_MENU_UP) ||
		(In_IsBtnPressed(IN_BTN_MENU_UP) && Sys_TimerExpired(btnTimer, MENU_FAST_UPDATE_RATE_MS)))
	{
		isPressed = true;
		btnTimer = Sys_GetTick();
	}
	return (isPressed);
}

/******************************************************************************
* Menu_GetNextItemDown
*	Get next item down in a menu list.
******************************************************************************/
void Menu_GetNextItemDown (menu_page_t *page)
{
	//get next item in list, if last one, wrap around to top
	page->current++;
	if (page->current >= page->numItems)
	{
		page->current = 0;
		page->top = 0;
	}
	//if current item moves off screen, adjust top item
	else if (page->current >= (page->top + MENU_NUM_LINES))
	{
		page->top++;
	}
}

/******************************************************************************
* Menu_GetNextItemDown
*	Get next item up in a menu list.
******************************************************************************/
void Menu_GetNextItemUp (menu_page_t *page)
{
	//if first item in list, wrap around to bottom
	if (page->current == 0)
	{
		page->current = page->numItems - 1;
		page->top = (page->numItems < MENU_NUM_LINES) ? 0 : (page->numItems - MENU_NUM_LINES);
	}
	//get previous item, if item moves off screen, adjust top item
	else
	{
		page->current--;
		if (page->current < page->top)
		{
			page->top--;
		}
	}
}

/******************************************************************************
* Menu_ScrollListDown
*	Scroll list down one item (move top of list down). If at bottom of list,
*	wrap around to first item.
******************************************************************************/
void Menu_ScrollListDown (menu_page_t *page)
{
	page->top++;
	if (page->top >= page->numItems)
	{
		page->top = 0;
	}
	page->current = page->top;
}

/******************************************************************************
* Menu_ScrollListUp
*	Scroll list up one item (move top of list up). If at top of list, wrap
*	around to last item.
******************************************************************************/
void Menu_ScrollListUp (menu_page_t *page)
{
	if (page->top == 0)
	{
		page->top = page->numItems - 1;
	}
	else
	{
		page->top--;
	}
	page->current = page->top;
}

/******************************************************************************
* Menu_EditUint8
*	Edit a menu item uint8_t value. The mode will indicate which direction to
*	modify value. The value will edited by step size. If edited value is
*	out-of-bounds, it will wrap around.
******************************************************************************/
void Menu_EditUint8 (uint8_t *pVal, uint8_t step, uint8_t min, uint8_t max, menu_edit_val_t mode)
{
	if (mode == MENU_EDIT_INCREASE)
	{
		*pVal += step;
		if ((*pVal > max) || (*pVal < min))
		{
			*pVal = min;
		}
	}
	else if (mode == MENU_EDIT_DECREASE)
	{
		*pVal -= step;
		if ((*pVal < min) || (*pVal > max))
		{
			*pVal = max;
		}
	}
}

/******************************************************************************
* Menu_EditUint16
*	Edit a menu item uint16_t value. The mode will indicate which direction to
*	modify value. The value will edited by step size. If edited value is
*	out-of-bounds, it will wrap around.
******************************************************************************/
void Menu_EditUint16 (uint16_t *pVal, uint16_t step, uint16_t min, uint16_t max, menu_edit_val_t mode)
{
	if (mode == MENU_EDIT_INCREASE)
	{
		*pVal += step;
		if ((*pVal > max) || (*pVal < min))
		{
			*pVal = min;
		}
	}
	else if (mode == MENU_EDIT_DECREASE)
	{
		*pVal -= step;
		if ((*pVal < min) || (*pVal > max))
		{
			*pVal = max;
		}
	}
}

/******************************************************************************
* Menu_EditUint32
*	Edit a menu item uint32_t value. The mode will indicate which direction to
*	modify value. The value will edited by step size. If edited value is
*	out-of-bounds, it will wrap around.
******************************************************************************/
void Menu_EditUint32 (uint32_t *pVal, uint32_t step, uint32_t min, uint32_t max, menu_edit_val_t mode)
{
	if (mode == MENU_EDIT_INCREASE)
	{
		*pVal += step;
		if ((*pVal > max) || (*pVal < min))
		{
			*pVal = min;
		}
	}
	else if (mode == MENU_EDIT_DECREASE)
	{
		*pVal -= step;
		if ((*pVal < min) || (*pVal > max))
		{
			*pVal = max;
		}
	}
}

/******************************************************************************
* Menu_EditInt32
*	Edit a menu item int32_t value. The mode will indicate which direction to
*	modify value. The value will edited by step size. If edited value is
*	out-of-bounds, it will wrap around.
******************************************************************************/
void Menu_EditInt32 (int32_t *pVal, int32_t step, int32_t min, int32_t max, menu_edit_val_t mode)
{
	if (mode == MENU_EDIT_INCREASE)
	{
		*pVal += step;
		if ((*pVal > max) || (*pVal < min))
		{
			*pVal = min;
		}
	}
	else if (mode == MENU_EDIT_DECREASE)
	{
		*pVal -= step;
		if ((*pVal < min) || (*pVal > max))
		{
			*pVal = max;
		}
	}
}
