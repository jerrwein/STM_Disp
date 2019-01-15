/*
 * oled.h
 *
 *  Created on: Nov 29, 2018
 *      Author: jweinzierl
 */

#ifndef OLED_H_
#define OLED_H_

#include "font5x7.h"

/*
*	DEFINITIONS
**********************************************************/
#define OLED_PIX_WIDTH		(128u)		//num horizontal pixels
#define OLED_PIX_HEIGHT		(64u)		//num vertical pixels
//
#define OLED_LEFT_X			(0u)
#define OLED_CENTER_X		(63u)
#define OLED_RIGHT_X		(127u)
//
#define OLED_TOP_Y			(0u)
#define OLED_CENTER_Y		(31u)
#define OLED_BOTTOM_Y		(63u)

//font sizes
#define OLED_CHAR_HEIGHT	(FONT5X7_CHAR_HEIGHT)
#define OLED_CHAR_WIDTH		(FONT5X7_CHAR_WIDTH + 1) //+1 for space between characters

//text character lines
//- each line sets the top edge of character glyphs
//- line 1 is top line, line 7 is bottom line
#define OLED_LINE_HEIGHT	(9u)		//FONT5X7_CHAR_HEIGHT
#define OLED_NUM_LINES		(7u)
#define OLED_LINE_1			(1u)
#define OLED_LINE_2			(10u)
#define OLED_LINE_3			(19u)
#define OLED_LINE_4			(28u)
#define OLED_LINE_5			(37u)
#define OLED_LINE_6			(46u)
#define OLED_LINE_7			(55u)

//set horizontal alignment columns
//- each column sets left edge of character glyphs
//- column 1 is far left, column 21 is far right
#define OLED_COL_WIDTH		(6u)		//FONT5X7_CHAR_WIDTH + 1 for space
#define OLED_NUM_COLS		(21u)
#define OLED_COL_1			(1u)
#define OLED_COL_2			(7u)
#define OLED_COL_3			(13u)
#define OLED_COL_4			(19u)
#define OLED_COL_5			(25u)
#define OLED_COL_6			(31u)
#define OLED_COL_7			(37u)
#define OLED_COL_8			(43u)
#define OLED_COL_9			(49u)
#define OLED_COL_10			(55u)
#define OLED_COL_11			(61u)
#define OLED_COL_12			(67u)
#define OLED_COL_13			(73u)
#define OLED_COL_14			(79u)
#define OLED_COL_15			(85u)
#define OLED_COL_16			(91u)
#define OLED_COL_17			(97u)
#define OLED_COL_18			(103u)
#define OLED_COL_19			(109u)
#define OLED_COL_20			(115u)
#define OLED_COL_21			(121u)

//Oled_PutString flags
//-----------------------------------------------
//default alignment is ALIGN_LEFT and ALIGN_TOP:
//- x,y marks upper left corner of glyph)
//-----------------------------------------------
#define OLED_FLAG_ALIGN_HOR			(1u << 0)	//align hor. center of glyph to x-coord
#define OLED_FLAG_ALIGN_VERT		(1u << 1)	//align vert. center of glyph to y-coord
#define OLED_FLAG_ALIGN_RIGHT		(1u << 2)	//align right edge of glyph to x-coord
#define OLED_FLAG_ALIGN_BOTTOM		(1u << 3)	//align bottom edge of glyph to y-coord
#define OLED_FLAG_WRAP_STRING		(1u << 4)	//wrap string to next line
#define OLED_FLAG_INVERT			(1u << 5)	//invert all pixels in glyph



#define SSD1309_SET_CONTRAST					(0x81u)
#define SSD1309_SET_DISPLAY_MODE_NORMAL			(0xA4u)
#define SSD1309_SET_DISPLAY_MODE_ALL_ON			(0xA5u)
#define SSD1309_SET_DISPLAY_MODE_INVERSE_OFF	(0xA6u)
#define SSD1309_SET_DISPLAY_MODE_INVERSE_ON		(0xA7u)
#define SSD1309_SET_DISPLAY_OFF					(0xAEu)
#define SSD1309_SET_DISPLAY_ON					(0xAFu)
#define SSD1309_SET_ADDRESS_MODE				(0x20u)	//see addressing modes
#define SSD1309_SET_COLUMN_ADDRESS				(0x21u)
#define SSD1309_SET_PAGE_ADDRESS				(0x22u)
#define SSD1309_SET_COLUMN_REMAP_OFF			(0xA0u)	//scans left to right
#define SSD1309_SET_COLUMN_REMAP_ON				(0xA1u)	//scans right to left
#define SSD1309_SET_ROW_REMAP_OFF				(0xC0u)	//scans bottom to top
#define SSD1309_SET_ROW_REMAP_ON				(0xC8u)	//scans top to bottom
#define SSD1309_SET_COM_HW_CFG					(0xDAu)	//see hw config options

#define SSD1309_ADDRESS_MODE_HORIZONTAL			(0x00u)
#define SSD1309_ADDRESS_MODE_VERTICAL			(0x01u)
#define SSD1309_ADDRESS_MODE_PAGE				(0x02u)		//default

#define SSD1309_COM_HW_CFG_SEQUENTIAL_NO_REMAP		(0x02u)
#define SSD1309_COM_HW_CFG_ALTERNATIVE_NO_REMAP		(0x12u)		//default
#define SSD1309_COM_HW_CFG_SEQUENTIAL_AND_REMAP		(0x22u)
#define SSD1309_COM_HW_CFG_ALTERNATIVE_AND_REMAP	(0x32u)

#define SSD1309_NUM_ROWS			(64u)
#define SSD1309_NUM_COLS			(128u)
#define SSD1309_ROWS_PER_PAGE		(8u)		//bits
#define SSD1309_NUM_PAGES			(8u)		//num_rows / rows_per_page
#define SSD1309_FIRST_PAGE			(0u)
#define SSD1309_LAST_PAGE			(SSD1309_NUM_PAGES - 1u)
#define SSD1309_FIRST_COL			(0u)
#define SSD1309_LAST_COL			(SSD1309_NUM_COLS - 1u)

#define OLED_PIX_WIDTH		(128u)		//num horizontal pixels
#define OLED_PIX_HEIGHT		(64u)		//num vertical pixels
#define OLED_MAP_NUM_PAGES		SSD1309_NUM_PAGES
#define OLED_MAP_NUM_COLS		SSD1309_NUM_COLS

#define DS2482_BUFFER_SIZE	4
#define OLED_DATA_BFR_SIZE		(OLED_MAP_NUM_PAGES * OLED_MAP_NUM_COLS)
#define OLED_CMD_BFR_SIZE		6

//default alignment is ALIGN_LEFT and ALIGN_TOP:
//- x,y marks upper left corner of glyph)
//-----------------------------------------------
#define OLED_FLAG_ALIGN_HOR			(1u << 0)	//align hor. center of glyph to x-coord
#define OLED_FLAG_ALIGN_VERT		(1u << 1)	//align vert. center of glyph to y-coord
#define OLED_FLAG_ALIGN_RIGHT		(1u << 2)	//align right edge of glyph to x-coord
#define OLED_FLAG_ALIGN_BOTTOM		(1u << 3)	//align bottom edge of glyph to y-coord
#define OLED_FLAG_WRAP_STRING		(1u << 4)	//wrap string to next line
#define OLED_FLAG_INVERT			(1u << 5)	//invert all pixels in glyph

// Function prototypes
int32_t OLED_Init(int32_t len);
//void oled_clear_screen (bool refresh);
//void oled_fill_screen (bool refresh);
//void oled_draw_test_pattern_1(bool refresh);
//void oled_draw_test_pattern_2(bool refresh);
//void oled_hard_delay_us (uint32_t usecs);


void Oled_ClearScreenOrig (bool refresh);
void Oled_DrawGlyphOrig (uint32_t x, uint32_t y, const uint8_t *glyph, uint32_t flags);
void Oled_PutStringOrig (uint32_t x, uint32_t y, const char *str, uint32_t flags);
void Oled_PutChar (uint32_t x, uint32_t y, uint8_t ch, uint32_t flags);
void Oled_RefreshOrig (void);
void Oled_DrawHline (uint32_t x, uint32_t y, uint32_t width);

/* Called via an interrupt - keep it lean */
void oled_DMA_SPI_TxCpltCallback(void);

#endif /* OLED_H_ */
