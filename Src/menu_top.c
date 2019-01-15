/******************************************************************************
* menu_top.c
*
* Application specific implementation for the top level menu.
*
* Copyright (c) 2014 Nilfisk-Advance, Inc. All rights reserved.
*
* Version History
*------------------------------------------------------------------------------
* 0.01, 21Sep2014, CParker
*	- file created
*
******************************************************************************/
#include "stdint.h"
#include "stdbool.h"


#include "system.h"
#include "menus.h"
//#include "modes.h"
#include "display.h"
#include "oled.h"
#include "icons.h"
#include "inputs.h"
#include "text.h"
//#include "faults.h"
//#include "scrub.h"
//#include "recovery.h"
//#include "debug.h"
//#include <string.h>


/*
*	Defines
*******************************************************************************/

/*
*	Types
*******************************************************************************/
//top menu table
static const struct top_item_table
{
	text_id_t	labelId;
	menu_id_t	menuId;

} mk_topItemTable[MENU_NUM_TOP_ITEMS] =
{
	[MENU_TOP_ITEM_HOURS]	= { TEXT_ID_HOURS			,MENU_ID_HOURS },
	[MENU_TOP_ITEM_FAULTS]	= { TEXT_ID_FAULTS			,MENU_ID_FAULTS },
	[MENU_TOP_ITEM_KEYS]	= { TEXT_ID_KEYS			,MENU_ID_KEYS },
	[MENU_TOP_ITEM_SERVICE]	= { TEXT_ID_SERVICE			,MENU_ID_SERVICE },
	[MENU_TOP_ITEM_OPTIONS]	= { TEXT_ID_OPTIONS			,MENU_ID_OPTIONS },
	[MENU_TOP_ITEM_CONFIG]	= { TEXT_ID_CONFIGURATION	,MENU_ID_CONFIG },
	[MENU_TOP_ITEM_SYSTEM]	= { TEXT_ID_SYSTEM			,MENU_ID_SYSTEM },
};

/*
*	Variables
*******************************************************************************/

/*
*	Private Function Prototypes
*******************************************************************************/
static bool m_IsValidItem (uint32_t item);


/******************************************************************************
* TopMenu_Load
*	Load the top level menu.
******************************************************************************/
void TopMenu_Load (menu_page_t *page, uint32_t startingItem)
{
// jmw	assert(MENU_NUM_TOP_ITEMS <= MENU_ITEM_LIST_MAX_SIZE);
	
	if (page->menuId != MENU_ID_TOP)
	{
		page->current = 0;
		page->top = 0;
		page->numItems = 0;

		//create a list of valid items
		uint32_t i;
		for (i = 0; i < MENU_NUM_TOP_ITEMS; i++)
		{
			if (m_IsValidItem(i))
			{
				page->items[page->numItems] = i;
				page->numItems++;
			}
		}
		
		//set starting item
		for (i = 0; i < page->numItems; i++)
		{
			if (page->items[i] == startingItem)
			{
				break;
			}
			Menu_GetNextItemDown(page);
		}
	}
}

/******************************************************************************
* TopMenu_Run
*	Draw and run the top level menu.
******************************************************************************/
void TopMenu_Run (menu_page_t *page)
{
	//check user input
	//----------------------------------
	//left btn
	//- ignore, this is top level
	if (In_WasBtnPressed(IN_BTN_MENU_LEFT))
	{
		asm("nop");
	}
	//up btn
	//- select next item up
	else if (Menu_UpBtnPressedOrHeld())
	{
		Menu_GetNextItemUp(page);
		page->drawScreen = true;
	}
	//down btn
	//- select next item down
	else if (Menu_DownBtnPressedOrHeld())
	{
		Menu_GetNextItemDown(page);
		page->drawScreen = true;
	}
	//right btn
	//- user selected option
	else if (In_WasBtnPressed(IN_BTN_MENU_RIGHT))
	{
		Menu_SetMenu(mk_topItemTable[page->items[page->current]].menuId, 0);
	}
	
	//draw the screen
	//----------------------------------
	if (page->drawScreen)
	{
		page->drawScreen = false;
		Oled_ClearScreenOrig(false);
		
		Display_DrawHeader(Text_Get(TEXT_ID_MENU), MENU_PAGE_1);

		uint32_t item = page->top;
		for (uint32_t line = MENU_FIRST_LINE; line < MENU_NUM_LINES; line++)
		{
			if (item < page->numItems)
			{
				Display_DrawLineLabel(line, Text_Get(mk_topItemTable[page->items[item]].labelId), DISPLAY_FLAG_INDENT);
			}
			item++;
		}

		Display_DrawLineCursor(page->current - page->top);
		Display_DrawMenuXofY((page->current + 1), page->numItems);
		Display_DrawFooter(DISPLAY_FOOTER_EXIT_SELECT);
		
		Oled_RefreshOrig();
	}	
}

/******************************************************************************
* m_IsValidItem
*	Check for validity of a menu item.
******************************************************************************/
static bool m_IsValidItem (uint32_t item)
{
	bool isValid = true;
	
	//keys and options menus only available to supervisor and up and if
	//in no active mode
	if ((item == MENU_TOP_ITEM_KEYS) || (item == MENU_TOP_ITEM_OPTIONS))
	{
// jmw		if ((Sys_GetUserType() < SYS_USER_SUPERVISOR) ||
// jmw			Mode_IsActiveMode())
//jmw		{
			isValid = false;
// jmw		}
	}
	//service menu only available to technician and up
	else if (item == MENU_TOP_ITEM_SERVICE)
	{
	// jmw	if (Sys_GetUserType() < SYS_USER_TECHNICIAN)
	// jmw	{
			isValid = false;
	// jmw	}
	}
	//configuration menu only available to technician and up and if
	//in no active mode
	else if (item == MENU_TOP_ITEM_CONFIG)
	{
	// jmw	if ((Sys_GetUserType() < SYS_USER_TECHNICIAN) ||
		// jmw	Mode_IsActiveMode())
// jmw		{
			isValid = false;
// jmw		}
	}
	
	return (isValid);
}
