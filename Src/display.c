/*
 * display.c
 *
 *  Created on: Jan 10, 2019
 *      Author: jweinzierl
 */

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"

#include "system.h"
#include "inputs.h"
#include "detergent.h"
#include "icons.h"
//#include "font5x7.h"
#include "oled.h"
#include "display.h"
#include "text.h"
#include "menus.h"

/*
*	Defines
**********************************************************/
// Display update times
#define DISPLAY_UPDATE_RATE_MS		50
// Default screen refresh rate
#define DISPLAY_REFRESH_RATE_MS		250

// Text buffer
static uint8_t m_bfr[32];

//display line oled row look-up table
static const uint32_t mk_lineY[DISPLAY_LINES_PER_SCREEN] =
{
	[DISPLAY_LINE_1] = DISPLAY_LINE_1_Y,
	[DISPLAY_LINE_2] = DISPLAY_LINE_2_Y,
	[DISPLAY_LINE_3] = DISPLAY_LINE_3_Y,
	[DISPLAY_LINE_4] = DISPLAY_LINE_4_Y,
};

// Standard screen icon locations
//- x-pixel location to center icon on left side of screen
#define DISPLAY_LEFT_SIDE_CENTER_X		36

// Private functions
static void m_DrawChargingScreen(void);
static void m_DrawDetergentLevel (void);
static void m_DrawFlashingIconScreen(const uint8_t *icon);
static void m_DrawBattery (void);
static void m_DrawSolutionTank (void);

// Externals
extern osMessageQId DisplayQueueHandle;
extern osMutexId uart2_tx_buffer_mutexHandle;

// Modes
static display_mode_t m_mode;

// Timers
static uint32_t m_1sTimer;
static uint32_t m_refreshTimer;

// Flags
static struct
{
	bool refresh;
	bool tick500ms;
	bool tick500msHi;
	bool tick1s;
	bool tick1sHi;

} m_flags;

// Mutex (data lock) for oled map access
xSemaphoreHandle m_oledMutex;
// Semaphore trigger for updating display
xSemaphoreHandle m_onRefresh;


uint32_t display_init (void)
{
	m_mode = DISPLAY_MODE_NONE;

#if 1
	// Create mutex for accessing memory map
	m_oledMutex = xSemaphoreCreateMutex();
	if (m_oledMutex == NULL)
	{
		return (-1);
	}

	// Create semaphore for updating display
	vSemaphoreCreateBinary(m_onRefresh);
	if (m_onRefresh == NULL)
	{
		return (-2);
	}
#endif

	return 0;
}

void run_display_task (void const * argument)
{
	uint32_t 			u32_cnt = 0;
	display_mode_t		mode;
	bool 				newMode;
	/* Infinite loop */
	for(;;)
	{
		if (xQueueReceive(DisplayQueueHandle, &mode, 2*DISPLAY_UPDATE_RATE_MS) == pdPASS)	// 50ms.
		{
			xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
			printf("Display: mode(%d) change taken from queue: %ld\n", mode, ++u32_cnt);
			xSemaphoreGive(uart2_tx_buffer_mutexHandle);

			m_mode = mode;
			newMode = true;

			// new mode so reset timers
			m_1sTimer = Sys_GetTick();
			m_flags.tick500ms = true;
			m_flags.tick500msHi = true;
			m_flags.tick1s = true;
			m_flags.tick1sHi = true;
			m_refreshTimer = Sys_GetTick();
			m_flags.refresh = true;
		}
		else	// Not new mode so update timers
		{
			// Update screen refresh timer
			if (Sys_TimerExpired(m_refreshTimer, 2*DISPLAY_REFRESH_RATE_MS))	// 250ms.
			{
				m_refreshTimer = Sys_GetTick();
				m_flags.refresh = true;
			}
			// Update timer ticks
			if (Sys_TimerExpired(m_1sTimer, 500))
			{
				m_1sTimer = Sys_GetTick();
				m_flags.tick500ms = true;
				m_flags.tick500msHi = !m_flags.tick500msHi;
				if (m_flags.tick500msHi)
				{
					m_flags.tick1s = true;
					m_flags.tick1sHi = !m_flags.tick1sHi;
				}
			}
		}

		// Draw screen according to mode
		//------------------------------
		// Some other module has control of screen so do nothing
		if (m_mode == DISPLAY_MODE_NONE)
		{
			asm("nop");

		}
		// Draw battery charging mode screen
		//- show charging battery icon
		else if (m_mode == DISPLAY_MODE_CHARGING)
		{
			asm("nop");
			m_DrawChargingScreen();
		}
		// Draw e-stop mode screen
		//- left side: flash e-stop icon
		//- right side: standard items
		else if (m_mode == DISPLAY_MODE_ESTOP)
		{
			m_DrawFlashingIconScreen(gk_iconEstop);
		}
		else if (m_mode == DISPLAY_MODE_RTF)
		{
			m_DrawSolutionTank();
		}
		else if (m_mode == DISPLAY_MODE_PURGE)
		{
			m_DrawDetergentLevel();
		}
		else if (m_mode == DISPLAY_MODE_MENU)
		{
			Menu_Run();
		}
	}

}


/******************************************************************************
* m_DrawChargingScreen
*	Private: Draw charging battery icon: battery with flashing level bars. Draw
*	full battery icon when charging is complete.
******************************************************************************/
static void m_DrawChargingScreen (void)
{
	static uint32_t fillState = 5;

	bool drawScreen = true;

	//update screen every 1s
	if (m_flags.tick1s)
	{
		Oled_ClearScreenOrig(false);

		//if battery fully charged, flash full battery symbol
//		if (In_GetBatteryLevel() >= IN_BATTERY_LEVEL_5)
		if (true)
		{
			fillState = 5;

			if (m_flags.tick1sHi)
			{
				drawScreen = false;
			}
		}
		//if battery still charging, cycle through battery level bars
		else
		{
			if ((++fillState) > 5)
			{
				fillState = 0;
			}
		}

		if (drawScreen)
		{
			//draw empty battery icon
			uint32_t x = 34;
			uint32_t y = 14;
			Oled_DrawGlyphOrig(x, y, gk_iconCharging, 0);

			x += 4;
			y += 7;

			//draw fill icons
			for (uint32_t i = 0; i < fillState; i++)
			{
				Oled_DrawGlyphOrig(x, y, gk_iconChargingBar, 0);
				x += 11;
			}
		}

		Oled_RefreshOrig();
	//	xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
	//	printf("m_DrawChargingScreen:done\n");
	//	xSemaphoreGive(uart2_tx_buffer_mutexHandle);
	}
}


/******************************************************************************
* m_DrawSolutionTank
*	Private: Draw solution tank level at bottom right of screen below battery.
******************************************************************************/
static void m_DrawSolutionTank (void)
{
	uint32_t x = 97;
	uint32_t y = 41;

	in_sol_tank_level_t level = IN_SOL_TANK_LEVEL_2; // jmwIn_GetSolTankLevel();

	//if empty, flash empty tank symbol
	if (level == IN_SOL_TANK_LEVEL_0)
	{
		if (m_flags.tick1sHi)
		{
			Oled_DrawGlyphOrig(x, y, gk_iconTank, 0);
			Oled_DrawGlyphOrig((x - 8), (y + 1), gk_iconTankEmpty, 0);
		}
	}
	//if not empty, draw level indicators
	else
	{
		//draw tank
		Oled_DrawGlyphOrig(x, y, gk_iconTank, 0);
		x += 2;
		y += 19;

		//draw bottom part of level indicator
		Oled_DrawGlyphOrig(x, y, gk_iconTankLevelBtm, 0);

		//draw mid-level indicators
		if (level > IN_SOL_TANK_LEVEL_1)
		{
			y -= Icon_Height(gk_iconTankLevelMid);
			Oled_DrawGlyphOrig(x, y, gk_iconTankLevelMid, 0);
		}
		if (level > IN_SOL_TANK_LEVEL_2)
		{
			y -= Icon_Height(gk_iconTankLevelMid);
			Oled_DrawGlyphOrig(x, y, gk_iconTankLevelMid, 0);
		}
		if (level > IN_SOL_TANK_LEVEL_3)
		{
			y -= Icon_Height(gk_iconTankLevelMid);
			Oled_DrawGlyphOrig(x, y, gk_iconTankLevelMid, 0);
		}

		//draw top part of level indicator
		y -= Icon_Height(gk_iconTankLevelTop);
		Oled_DrawGlyphOrig(x, y, gk_iconTankLevelTop, 0);
	}
	Oled_RefreshOrig();
}

/******************************************************************************
* m_DrawDetergentLevel
*	Private: Draw detergent symbol, detergent ratio and detergent level bars.
******************************************************************************/
static void m_DrawDetergentLevel (void)
{
	uint32_t x = 0;
	uint32_t y_btm = OLED_PIX_HEIGHT;	//use for bottom y reference

	//draw detergent symbol
	uint32_t y = y_btm - Icon_Height(gk_iconDetergentFlask);
	Oled_DrawGlyphOrig(x, y, gk_iconDetergentFlask, 0);

	//only draw the ratio and level bars if detergent is enabled
//	if (Det_IsEnabled())
	if (true)
	{
	//	det_level_t level = Det_GetLevel();
	//	det_ratio_t ratio = Det_GetRatio(level);
		det_level_t level = DET_LEVEL_HIGH;
		det_ratio_t ratio = DET_RATIO_26;
		uint32_t ratioFlags = OLED_FLAG_ALIGN_HOR;

		//if editing detergent ratio, flash the temporary value
//		if (ScrubMode_EditingDetergent())
		if (false)
		{
			ratio = DET_RATIO_26;
			if (!m_flags.tick500msHi)
			{
				ratioFlags |= OLED_FLAG_INVERT;
			}
		}

		//draw detergent ratio string
		y = y_btm - FONT5X7_CHAR_HEIGHT;
//		if (g_paramConfig.brand == PARAM_CFG_BRAND_NILFISK)
//		{
			x += 29;
			Oled_PutStringOrig(x, y, Det_GetPercentStr(ratio), ratioFlags);
			x += 1;
//		}
//		else
//		{
//			x += 30;
//			Oled_PutString(x, y, Det_GetRatioStr(ratio), ratioFlags);
//		}

		//draw detergent level bars
		x += 16;
		y = y_btm - Icon_Height(gk_iconLevel1Fill);
		Oled_DrawGlyphOrig(x, y, gk_iconLevel1Fill, 0);
		x += 6;
		y = y_btm - Icon_Height(gk_iconLevel3Box);
		const uint8_t *icon = (level > DET_LEVEL_LOW) ? gk_iconLevel3Fill : gk_iconLevel3Box;
		Oled_DrawGlyphOrig(x, y, icon, 0);
	}
	//detergent disabled so draw empty level bars
	else
	{
		x += 46;
		y = y_btm - Icon_Height(gk_iconLevel1Box);
		Oled_DrawGlyphOrig(x, y, gk_iconLevel1Box, 0);
		x += 6;
		y = y_btm - Icon_Height(gk_iconLevel3Box);
		Oled_DrawGlyphOrig(x, y, gk_iconLevel3Box, 0);
	}
	Oled_RefreshOrig();
}




/******************************************************************************
* m_DrawFlashingIconScreen
*	Private: Draw standard screen with flashing icon centered in middle of left
*	side of screen and standard right side items.
******************************************************************************/
static void m_DrawFlashingIconScreen (const uint8_t *icon)
{
	//update screen every 1s
	if (m_flags.tick1s)
	{
		Oled_ClearScreenOrig(false);

		//draw icon in middle of left side of screen
		if (m_flags.tick1sHi)
		{
			Oled_DrawGlyphOrig(DISPLAY_LEFT_SIDE_CENTER_X, OLED_CENTER_Y, icon, (OLED_FLAG_ALIGN_HOR | OLED_FLAG_ALIGN_VERT));
		}
// jmw		m_DrawSystemHours();
// jmw		m_DrawFaultCodes();
		m_DrawBattery();
		m_DrawSolutionTank();

		Oled_RefreshOrig();
	}
}

/******************************************************************************
* Display_DrawMenuXofY
*	Draw the (X/Y) value in upper right corner of menu screen.
******************************************************************************/
void Display_DrawMenuXofY (uint32_t item, uint32_t total)
{
	sprintf (m_bfr, "(%d/%d)", (int)item, (int)total);
	Oled_PutStringOrig(DISPLAY_LINE_RIGHT_X, DISPLAY_HEADER_Y, m_bfr, OLED_FLAG_ALIGN_RIGHT);
}

/******************************************************************************
* m_DrawBattery
*	Private: Draw battery level on right side below fault codes.
******************************************************************************/
static void m_DrawBattery (void)
{
	uint32_t x = 98;
	uint32_t y = 22;
	uint32_t barWidth = Icon_Width(gk_iconBatteryBar) + 1;

	// Draw empty battery
	//- only draw if level is above the cutout region
	in_battery_level_t level = IN_BATTERY_LEVEL_4; // jmwIn_GetBatteryLevel();
	if (level >= IN_BATTERY_LEVEL_0)
	{
		Oled_DrawGlyphOrig(x, y, gk_iconBattery, 0);
		x += 2;
		y += 4;

		// Draw battery level bars
		for (in_battery_level_t i = IN_BATTERY_LEVEL_0; i < level; i++)
		{
			Oled_DrawGlyphOrig(x, y, gk_iconBatteryBar, 0);
			x += barWidth;
		}
	}
}

/******************************************************************************
* Display_DrawLineCursor
*	Draw a line cursor on the designated text line.
******************************************************************************/
inline void Display_DrawLineCursor (display_line_t line)
{
	Oled_PutChar(0, mk_lineY[line], (char)FONT5X7_CURSOR, 0);
}

/******************************************************************************
* Display_DrawLineLabel
*	Draw a label on the designated text line.
*	Line labels are left-aligned on the left edge of the screen.
******************************************************************************/
void Display_DrawLineLabel (display_line_t line, const char *labelStr, uint32_t flags)
{
	uint32_t lineX = (flags & DISPLAY_FLAG_INDENT) ? DISPLAY_LINE_LEFT_X : 0;
	uint32_t oledFlags = (flags & DISPLAY_FLAG_INVERT) ? OLED_FLAG_INVERT : 0;
	Oled_PutStringOrig(lineX, mk_lineY[line], labelStr, oledFlags);
}

/******************************************************************************
* Display_DrawHeader
*	Draw the menu header. The header will contain the menu's title/label with
*	a horizontal separator bar below.
******************************************************************************/
void Display_DrawHeader (const char *title, uint32_t level)
{
	uint32_t x = 0;
	for (uint32_t i = 0; i < level; i++)
	{
		Oled_PutChar(x, DISPLAY_HEADER_Y, FONT5X7_RT_TRIANGLE, 0);
		x += FONT5X7_CHAR_WIDTH;
	}
	Oled_PutStringOrig(x, DISPLAY_HEADER_Y, title, 0);
	Oled_DrawHline(0, DISPLAY_HEADER_LINE_Y, OLED_PIX_WIDTH);
}


/******************************************************************************
* Display_DrawFooter
*	Draw the menu footer. The footer will contain menu 'instructions' with
*	a horizontal separator line above.
******************************************************************************/
void Display_DrawFooter (display_footer_t type)
{
	static char leftBfr[32];
	static char middleBfr[32];
	static char rightBfr[32];

	//draw separator line
	Oled_DrawHline(0, DISPLAY_FOOTER_LINE_Y, OLED_PIX_WIDTH);

	//back
	//----------------------------------
	//left:		back (L)
	//middle:
	//right:
	if (type == DISPLAY_FOOTER_BACK)
	{
		sprintf(leftBfr, "%c%s", (char)FONT5X7_LT_TRIANGLE, Text_Get(TEXT_ID_BACK));
		strcpy(middleBfr, "");
		strcpy(rightBfr, "");
	}
	//back-add
	//----------------------------------
	//left:		back (L)
	//middle:	add (U)
	//right:
	else if (type == DISPLAY_FOOTER_BACK_ADD)
	{
		sprintf(leftBfr, "%c%s", (char)FONT5X7_LT_TRIANGLE, Text_Get(TEXT_ID_BACK));
		sprintf(middleBfr, "%c%s", (char)FONT5X7_UP_TRIANGLE, Text_Get(TEXT_ID_ADD));
		strcpy(rightBfr, "");
	}
	//back-delete
	//----------------------------------
	//left:		back (L)
	//middle:	delete (U)
	//right:
	else if (type == DISPLAY_FOOTER_BACK_DELETE)
	{
		sprintf(leftBfr, "%c%s", (char)FONT5X7_LT_TRIANGLE, Text_Get(TEXT_ID_BACK));
		sprintf(middleBfr, "%c%s", (char)FONT5X7_UP_TRIANGLE, Text_Get(TEXT_ID_DELETE));
		strcpy(rightBfr, "");
	}
	//back-make_supervisor
	//----------------------------------
	//left:		back (L)
	//middle:
	//right:	make supervisor (U)	(won't fit in middle so put on right)
	else if (type == DISPLAY_FOOTER_BACK_MAKESUPERVISOR)
	{
		sprintf(leftBfr, "%c%s", (char)FONT5X7_LT_TRIANGLE, Text_Get(TEXT_ID_BACK));
		strcpy(middleBfr, "");
		sprintf(rightBfr, "%c%s", (char)FONT5X7_UP_TRIANGLE, Text_Get(TEXT_ID_MAKE_SUPERVISOR));
	}
	//back-scroll
	//----------------------------------
	//left:		back (L)
	//middle:	scroll (U/D)
	//right:
	else if (type == DISPLAY_FOOTER_BACK_SCROLL)
	{
		sprintf(leftBfr, "%c%s", (char)FONT5X7_LT_TRIANGLE, Text_Get(TEXT_ID_BACK));
		sprintf(middleBfr, "%c%s", (char)FONT5X7_UP_DN_TRIANGLES, Text_Get(TEXT_ID_SCROLL));
		strcpy(rightBfr, "");
	}
	//back-scroll-page
	//----------------------------------
	//left:		back (L)
	//middle:	scroll (U/D)
	//right:	page (R)
	else if (type == DISPLAY_FOOTER_BACK_SCROLL_PAGE)
	{
		sprintf(leftBfr, "%c%s", (char)FONT5X7_LT_TRIANGLE, Text_Get(TEXT_ID_BACK));
		sprintf(middleBfr, "%c%s", (char)FONT5X7_UP_DN_TRIANGLES, Text_Get(TEXT_ID_SCROLL));
		sprintf(rightBfr, "%c%s", (char)FONT5X7_RT_TRIANGLE, Text_Get(TEXT_ID_PAGE));
	}
	//back-select
	//----------------------------------
	//left:		back (L)
	//middle:	select (U/D/R)
	//right:
	else if (type == DISPLAY_FOOTER_BACK_SELECT)
	{
		sprintf(leftBfr, "%c%s", (char)FONT5X7_LT_TRIANGLE, Text_Get(TEXT_ID_BACK));
		sprintf(middleBfr, "%c%c%s", (char)FONT5X7_UP_DN_TRIANGLES, (char)FONT5X7_RT_TRIANGLE, Text_Get(TEXT_ID_SELECT));
		strcpy(rightBfr, "");
	}
	//back-select-save
	//----------------------------------
	//left:		back (L)
	//middle:	select (U/D)
	//right:	save (R)
	else if (type == DISPLAY_FOOTER_BACK_SELECT_SAVE)
	{
		sprintf(leftBfr, "%c%s", (char)FONT5X7_LT_TRIANGLE, Text_Get(TEXT_ID_BACK));
		sprintf(middleBfr, "%c%s", (char)FONT5X7_UP_DN_TRIANGLES, Text_Get(TEXT_ID_SELECT));
		sprintf(rightBfr, "%c%s", (char)FONT5X7_RT_TRIANGLE, Text_Get(TEXT_ID_SAVE));
	}
	//back-set
	//----------------------------------
	//left:		back (L)
	//middle:	set (U/D)
	//right:
	else if (type == DISPLAY_FOOTER_BACK_SET)
	{
		sprintf(leftBfr, "%c%s", (char)FONT5X7_LT_TRIANGLE, Text_Get(TEXT_ID_BACK));
		sprintf(middleBfr, "%c%s", (char)FONT5X7_UP_DN_TRIANGLES, Text_Get(TEXT_ID_SET));
		strcpy(rightBfr, "");
	}
	//cancel
	//----------------------------------
	//left:		cancel (L)
	//middle:
	//right:
	else if (type == DISPLAY_FOOTER_CANCEL)
	{
		sprintf(leftBfr, "%c%s", (char)FONT5X7_LT_TRIANGLE, Text_Get(TEXT_ID_CANCEL));
		strcpy(middleBfr, "");
		strcpy(rightBfr, "");
	}
	//cancel-clear
	//----------------------------------
	//left:		cancel (L)
	//middle:	clear (U)
	//right:
	else if (type == DISPLAY_FOOTER_CANCEL_CLEAR)
	{
		sprintf(leftBfr, "%c%s", (char)FONT5X7_LT_TRIANGLE, Text_Get(TEXT_ID_CANCEL));
		sprintf(middleBfr, "%c%s", (char)FONT5X7_UP_TRIANGLE, Text_Get(TEXT_ID_CLEAR));
		strcpy(rightBfr, "");
	}
	//cancel-edit-save
	//----------------------------------
	//left:		cancel (L)
	//middle:	edit (U/D)
	//right:	save (R)
	else if (type == DISPLAY_FOOTER_CANCEL_EDIT_SAVE)
	{
		sprintf(leftBfr, "%c%s", (char)FONT5X7_LT_TRIANGLE, Text_Get(TEXT_ID_CANCEL));
		sprintf(middleBfr, "%c%s", (char)FONT5X7_UP_DN_TRIANGLES, Text_Get(TEXT_ID_EDIT));
		sprintf(rightBfr, "%c%s", (char)FONT5X7_RT_TRIANGLE, Text_Get(TEXT_ID_SAVE));
	}
	//cancel-install
	//----------------------------------
	//left:		cancel (L)
	//middle:
	//right:	install (U)
	else if (type == DISPLAY_FOOTER_CANCEL_INSTALL)
	{
		sprintf(leftBfr, "%c%s", (char)FONT5X7_LT_TRIANGLE, Text_Get(TEXT_ID_CANCEL));
		strcpy(middleBfr, "");
		sprintf(rightBfr, "%c%s", (char)FONT5X7_UP_TRIANGLE, Text_Get(TEXT_ID_INSTALL));
	}
	//cancel-reset
	//----------------------------------
	//left:		cancel (L)
	//middle:
	//right:	reset (R)
	else if (type == DISPLAY_FOOTER_CANCEL_RESET)
	{
		sprintf(leftBfr, "%c%s", (char)FONT5X7_LT_TRIANGLE, Text_Get(TEXT_ID_CANCEL));
		strcpy(middleBfr, "");
		sprintf(rightBfr, "%c%s", (char)FONT5X7_RT_TRIANGLE, Text_Get(TEXT_ID_RESET));
	}
	//cancel-save
	//----------------------------------
	//left:		cancel (L)
	//middle:
	//right:	save (R)
	else if (type == DISPLAY_FOOTER_CANCEL_SAVE)
	{
		sprintf(leftBfr, "%c%s", (char)FONT5X7_LT_TRIANGLE, Text_Get(TEXT_ID_CANCEL));
		strcpy(middleBfr, "");
		sprintf(rightBfr, "%c%s", (char)FONT5X7_RT_TRIANGLE, Text_Get(TEXT_ID_SAVE));
	}
	//exit-select
	//----------------------------------
	//left:		exit (i)
	//middle:	select (U/D/R)
	//right:
	else if (type == DISPLAY_FOOTER_EXIT_SELECT)
	{
		sprintf(leftBfr, "%c %s", (char)FONT5X7_INFO, Text_Get(TEXT_ID_EXIT));
		sprintf(middleBfr, "%c%c%s", (char)FONT5X7_UP_DN_TRIANGLES, (char)FONT5X7_RT_TRIANGLE, Text_Get(TEXT_ID_SELECT));
		strcpy(rightBfr, "");
	}
	//----------------------------------
	else
	{
		strcpy(leftBfr, "");
		strcpy(middleBfr, "");
		strcpy(rightBfr, "");
	}

	Oled_PutStringOrig(0, DISPLAY_FOOTER_Y, leftBfr, 0);
	Oled_PutStringOrig(OLED_CENTER_X, DISPLAY_FOOTER_Y, middleBfr, OLED_FLAG_ALIGN_HOR);
	Oled_PutStringOrig(OLED_RIGHT_X, DISPLAY_FOOTER_Y, rightBfr, OLED_FLAG_ALIGN_RIGHT);
}

