/*
 * oled.c
 *
 *  Created on: Nov 29, 2018
 *      Author: jweinzierl
 */
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"

#include "system.h"
#include "oled.h"
#include "main.h"

const uint8_t mk_testPattern[OLED_MAP_NUM_PAGES] =
{
	0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff
};

// Mutex (data lock) for oled map access
extern xSemaphoreHandle m_oledMutex;
// Semaphore trigger for updating display
extern xSemaphoreHandle m_onRefresh;
extern osMutexId uart2_tx_buffer_mutexHandle;

uint8_t 		m_dataBfr[OLED_DATA_BFR_SIZE];
uint8_t			m_cmdBfr[OLED_CMD_BFR_SIZE];
uint8_t 		m_oledMap[OLED_MAP_NUM_COLS][OLED_MAP_NUM_PAGES];
__IO bool 		m_transferComplete;
uint32_t 		nr_transfer_complete_cbs = 0;
__IO uint32_t 	m_dummyToFoolOptimization;

static void oled_clear_screen (bool refresh);
static void oled_fill_screen (bool refresh);
static void oled_draw_test_pattern_1(bool refresh);
static void oled_draw_test_pattern_2(bool refresh);
static void m_write_cmd (uint16_t numBytes, bool use_cs);
static void m_write_data (void);
static void oled_hard_delay_us (uint32_t usecs);

void start_dma_transfer (uint8_t buf[], uint32_t nr_bytes);
static uint32_t m_GetStringWidth (const char *str);
static void m_FillBox (uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t flags, bool fill);

int32_t OLED_Init(int32_t len)
{
	uint32_t	waiting = 0;

	// Initial state */
	m_transferComplete = true;
	m_dummyToFoolOptimization  = 0;

	/* Raise SPI-nSS - may not be needed */
//	LL_GPIO_SetOutputPin(SPI1_nSS_GPIO_Port, SPI1_nSS_Pin);

	// Reset oled (clear reset line)
	LL_GPIO_ResetOutputPin(OLED_RESET_GPIO_Port, OLED_RESET_Pin);
	LL_mDelay(1);

	// Enable oled (release reset line)
	LL_GPIO_SetOutputPin(OLED_RESET_GPIO_Port, OLED_RESET_Pin);
	LL_mDelay(1);

	m_cmdBfr[0] = SSD1309_SET_ADDRESS_MODE;
	m_cmdBfr[1] = SSD1309_ADDRESS_MODE_VERTICAL;

	// Set display for 'upside-down'
	m_cmdBfr[2] = SSD1309_SET_ROW_REMAP_ON;		//btm->top
	m_cmdBfr[3] = SSD1309_SET_COLUMN_REMAP_ON;	//rt->lft
//	m_cmdBfr[2] = SSD1309_SET_ROW_REMAP_OFF;
//	m_cmdBfr[3] = SSD1309_SET_COLUMN_REMAP_OFF;

	// Set COM pins hardware configuration
	m_cmdBfr[4] = SSD1309_SET_COM_HW_CFG;
	m_cmdBfr[5] = SSD1309_COM_HW_CFG_ALTERNATIVE_NO_REMAP;

	// Send cmd data to OLED driver
	m_write_cmd(6, true /* use SPI_nSS */);
	oled_hard_delay_us(5);

	// Clear display memory
	oled_clear_screen (true);
//	oled_draw_test_pattern_1(true);

	// Turn display on
	m_cmdBfr[0] = SSD1309_SET_DISPLAY_ON;
	m_write_cmd(1, true);
	while (!m_transferComplete)
	{
		oled_hard_delay_us(2);
		if ((++waiting % 200000) == 0)
		{
			printf("m_WriteData(init): waiting...\n");
		}
	}

	/* Kluge to deal with premature xfer-complete */
	oled_hard_delay_us(5);
	LL_GPIO_SetOutputPin(SPI1_nSS_GPIO_Port, SPI1_nSS_Pin);

	return 0;
}

static void m_write_cmd (uint16_t numBytes, bool use_cs)
{
	__IO  uint32_t	waiting = 0;

	// Wait for previous xfer to complete
	while (!m_transferComplete)
	{
		oled_hard_delay_us(2);
		if ((++waiting % 250000) == 0)
		{
			printf("m_WriteData(c): waiting...\n");
		}
	}

	/* Kluge to deal with premature xfer-complete */
	oled_hard_delay_us(5);

	// Set data/cmd line for cmd
	LL_GPIO_ResetOutputPin(OLED_DC_GPIO_Port, OLED_DC_Pin);
	if (use_cs)
	{
		/* Lower SPI-nSS */
		LL_GPIO_ResetOutputPin(SPI1_nSS_GPIO_Port, SPI1_nSS_Pin);
	}

	// xmit data with DMA
	m_transferComplete = false;

//	printf ("m_write_cmd(%d): sending buffer...\n", numBytes );

#if 0
	/* Disable DMA prior to loading params */
	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
	LL_DMA_ConfigAddresses(DMA2, /*LL_DMA_CHANNEL_2*/ LL_DMA_STREAM_2,
						   (uint32_t)m_cmdBfr, LL_SPI_DMA_GetRegAddr(SPI1),
						   LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_SetDataLength(DMA2, /*LL_DMA_CHANNEL_2*/ LL_DMA_STREAM_2, numBytes);
	LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_2);
	LL_SPI_EnableDMAReq_TX(SPI1);
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);
#endif
	start_dma_transfer(m_cmdBfr, numBytes);

//	printf ("m_write_cmd(): sent, SPI:%ld, DMA:%ld.\n", LL_SPI_IsEnabled(SPI1), LL_SPI_IsEnabledDMAReq_TX(SPI1));

	/* Note: DMA-complete interrupt callback will raise SPI-nSS */
}

static void m_write_data (void)
{
	__IO  uint32_t 	i, j;
	__IO  uint32_t	waiting = 0;

	// Set row and column addresses
	m_cmdBfr[0] = SSD1309_SET_PAGE_ADDRESS;
	m_cmdBfr[1] = SSD1309_FIRST_PAGE;
	m_cmdBfr[2] = SSD1309_LAST_PAGE;
	m_cmdBfr[3] = SSD1309_SET_COLUMN_ADDRESS;
	m_cmdBfr[4] = SSD1309_FIRST_COL;
	m_cmdBfr[5] = SSD1309_LAST_COL;

	m_write_cmd(6, true);

	uint32_t n = 0;
	for (i = 0; i < OLED_MAP_NUM_COLS; i++)
	{
		for (j = 0; j < OLED_MAP_NUM_PAGES; j++)
		{
			m_dataBfr[n] = m_oledMap[i][j];
			n++;
		}
	}

	// Wait for previous xfer to complete
	while (!m_transferComplete)
	{
		oled_hard_delay_us(2);
		if ((++waiting % 250000) == 0)
		{
			printf("m_WriteData(d): waiting...\n");
		}
	}
	/* Kluge to deal with premature xfer-complete */
	oled_hard_delay_us(5);

	// Set data/cmd line for data
	LL_GPIO_SetOutputPin(OLED_DC_GPIO_Port, OLED_DC_Pin);

	/* Lower SPI-nSS */
	LL_GPIO_ResetOutputPin(SPI1_nSS_GPIO_Port, SPI1_nSS_Pin);

//	 printf ("m_write_data(): sending buffer...\n");
	// xmit data via DMA
	m_transferComplete = false;

#if 0
	/* Disable stream prior to loading new params */
	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
	LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_2,
					   	   (uint32_t)m_dataBfr, LL_SPI_DMA_GetRegAddr(SPI1),
						   LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_SetDataLength(DMA2, /*LL_DMA_CHANNEL_2*/ LL_DMA_STREAM_2, OLED_DATA_BFR_SIZE);
	LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_2);
	LL_SPI_EnableDMAReq_TX(SPI1);
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);
#endif

	start_dma_transfer(m_dataBfr, OLED_DATA_BFR_SIZE);

//	printf ("m_write_data(%ld): sent buffer.\n", Sys_GetTick());

	/* Note: DMA-complete interrupt callback will raise SPI-nSS */
}


static void oled_clear_screen (bool refresh)
{
	uint32_t	waiting = 0;

	// clear data map and display
	for (uint32_t i = 0; i < OLED_MAP_NUM_COLS; i++)
	{
		for (uint32_t j = 0; j < OLED_MAP_NUM_PAGES; j++)
		{
			m_oledMap[i][j] = 0;
		}
	}
	if (refresh)
	{
		m_write_data();
		while (!m_transferComplete)
		{
			oled_hard_delay_us(5);
			if ((++waiting % 250000) == 0)
			{
				printf("m_WriteData(cs): waiting...\n");
			}
		}

		/* Kluge to deal with premature xfer-complete */
		oled_hard_delay_us(10);
		LL_GPIO_SetOutputPin(SPI1_nSS_GPIO_Port, SPI1_nSS_Pin);
	}
}

/******************************************************************************
* Oled_ClearScreen
*	Erase memory map data and clear display.
******************************************************************************/
void Oled_ClearScreenOrig (bool refresh)
{
	// Clear data map and display
	xSemaphoreTake(m_oledMutex, portMAX_DELAY);
	for (uint32_t i = 0; i < OLED_MAP_NUM_COLS; i++)
	{
		for (uint32_t j = 0; j < OLED_MAP_NUM_PAGES; j++)
		{
			m_oledMap[i][j] = 0;
		}
	}
	xSemaphoreGive(m_oledMutex);

	if (refresh)
	{
		Oled_RefreshOrig();
	}
}


static void oled_fill_screen (bool refresh)
{
	uint32_t	waiting = 0;

	// set every pixel to full brightness
	for (uint32_t i = 0; i < OLED_MAP_NUM_COLS; i++)
	{
		for (uint32_t j = 0; j < OLED_MAP_NUM_PAGES; j++)
		{
			m_oledMap[i][j] = 0xff;
		}
	}
	if (refresh)
	{
		m_write_data();
		while (!m_transferComplete)
		{
			oled_hard_delay_us(5);
			if ((++waiting % 250000) == 0)
			{
				printf("m_WriteData(fs): waiting...\n");
			}
		}

		/* Kluge to deal with premature xfer-complete */
		oled_hard_delay_us(10);
		LL_GPIO_SetOutputPin(SPI1_nSS_GPIO_Port, SPI1_nSS_Pin);
	}
}

static void oled_draw_test_pattern_1 (bool refresh)
{
	uint16_t i, j, k;
	uint8_t  pattern[OLED_MAP_NUM_PAGES];
	uint8_t carryBitOut, carryBitIn;
	uint32_t	waiting = 0;

	// Copy test pattern
	for (k = 0; k < OLED_MAP_NUM_PAGES; k++)
	{
		pattern[k] = mk_testPattern[k];
	}

	// Cycle through each of the 128 columns
	for (i = 0; i < OLED_PIX_WIDTH; i++)
	{
		// Each pixel is 1-bit in memory map, 1-byte covers 8 pixels
		for (j = 0; j < OLED_MAP_NUM_PAGES; j++)
		{
			m_oledMap[i][j] = pattern[j];
		}

		// Shift test pattern down: 1-pixel (1-bit) for each row
		carryBitOut = pattern[OLED_MAP_NUM_PAGES-1] & 0x80;
		carryBitIn = carryBitOut;
		for (k=0; k < OLED_MAP_NUM_PAGES; k++)
		{
			carryBitOut = pattern[k] & 0x80;
			pattern[k] <<= 1;
			if (carryBitIn)
			{
				pattern[k] |= 0x01;
			}
			carryBitIn = carryBitOut;
		}
	}
	if (refresh)
	{
		m_write_data();
		while (!m_transferComplete)
		{
			oled_hard_delay_us(5);
			if ((++waiting % 250000) == 0)
			{
				printf("m_WriteData(tp1): waiting...\n");
			}
		}

		/* Kluge to deal with premature xfer-complete */
		oled_hard_delay_us(10);
		LL_GPIO_SetOutputPin(SPI1_nSS_GPIO_Port, SPI1_nSS_Pin);
	}
}

static void oled_draw_test_pattern_2 (bool refresh)
{
	int16_t i, j;
	uint8_t  pattern[OLED_MAP_NUM_PAGES];
	uint8_t carryBitOut, carryBitIn;
	uint32_t	waiting = 0;

	// Copy test pattern
	for (j = 0; j < (uint16_t)OLED_MAP_NUM_PAGES; j++)
	{
		pattern[j] = mk_testPattern[j];
	}

	// Cycle through each of the 128 columns
	for (i = 0; i < (uint16_t)OLED_PIX_WIDTH; i++)
	{
		// Each pixel is 1-bit in memory map, 1-byte covers 8 pixels
		for (j = 0; j < (uint16_t)OLED_MAP_NUM_PAGES; j++)
		{
			m_oledMap[i][j] = pattern[j];
		}

		// Shift test pattern up: 1-pixel (1-bit) for each row
		carryBitOut = pattern[0] & 0x01;
		carryBitIn = carryBitOut;
		for (j = OLED_MAP_NUM_PAGES-1; 0 <= j; j--)
		{
			carryBitOut = pattern[j] & 0x01;
			pattern[j] >>= 1;
			if (carryBitIn)
			{
				pattern[j] |= 0x80;
			}
			carryBitIn = carryBitOut;
		}
	}
	if (refresh)
	{
		m_write_data();
		while (!m_transferComplete)
		{
			oled_hard_delay_us(5);
			if ((++waiting % 250000) == 0)
			{
				printf("m_WriteData(tp2): waiting...\n");
			}
		}

		/* Kluge to deal with premature xfer-complete */
		oled_hard_delay_us(10);
		LL_GPIO_SetOutputPin(SPI1_nSS_GPIO_Port, SPI1_nSS_Pin);
	}
}

/******************************************************************************
* Oled_DrawGlyph
*	Convert a glyph bitmap and draw at assigned location in display memory map.
*	Update display (copy display map to oled) if requested.
*	Glyphs are drawn so that, unless otherwise specified by flags, the x,y
*	coordinates mark the upper left corner of glyph.
*
* x:		x-coordinate reference
* y:		y-coordinate reference
* glyph:	pointer to glyph
* flags:	see descriptions in .h
******************************************************************************/
void Oled_DrawGlyphOrig (uint32_t x, uint32_t y, const uint8_t *glyph, uint32_t flags)
{
	//get glyph bitmap size
	uint32_t width = glyph[0];
	uint32_t height = glyph[1];

	//assign starting x-coord
	//- default is left alignment
	//- if aligning to center or right and there's not enough room, start at 0
	uint32_t x1 = x;
	if (flags & OLED_FLAG_ALIGN_HOR)
	{
		uint32_t offset = width >> 1;
		x1 = (x >= offset) ? (x - offset) : 0;
	}
	else if (flags & OLED_FLAG_ALIGN_RIGHT)
	{
		x1 = (x >= width) ? (x - width) : 0;
	}
	x = x1;

	//assign starting y-coord
	//- default is top alignment
	//- if aligning to center or bottom and there's not enough room, start at 0
	uint32_t y1 = y;
	if (flags & OLED_FLAG_ALIGN_VERT)
	{
		uint32_t offset = height >> 1;
		y1 = (y >= offset) ? (y - offset) : 0;
	}
	else if (flags & OLED_FLAG_ALIGN_BOTTOM)
	{
		y1 = (y >= height) ? (y - height) : 0;
	}

	//calculate number of pages in glyph map
	//- 8 rows per page, add 1 if remainder
	uint32_t numPages = height >> 3;
	if (height & 0x07u)
	{
		numPages++;
	}

	//write data from glyph to memory map
	//- vertical addressing is used so glyph data should be organized from top
	//  to bottom, then left to right

	xSemaphoreTake(m_oledMutex, portMAX_DELAY);
	//cycle through each column of glyph
	for (uint32_t col = 0; col < width; col++)
	{
		//starting new column so reset memory map row index
		y = y1;

		//cycle through each page of glyph column
		for (uint32_t page = 0; page < numPages; page++)
		{
			//get glyph data (+2 is offset for width/height bytes in glyph map)
			uint8_t data = glyph[(col * numPages) + page + 2];

			//cycle through each bit(row) in page
			for (uint32_t bit = 0; bit < 8; bit++)
			{
				//set corresponding memory map bit same as glyph map bit
				uint8_t mapMask = 1u << (y & 0x07u);
				uint32_t mapPage = y >> 3;

				//if glyph bit is set...
				if (data & (1u << bit))
				{
					if (flags & OLED_FLAG_INVERT)
					{
						m_oledMap[x][mapPage] &= ~mapMask;
					}
					else
					{
						m_oledMap[x][mapPage] |= mapMask;
					}
				}
				//if glyph bit is clear...
				else if (flags & OLED_FLAG_INVERT)
				{
					m_oledMap[x][mapPage] |= mapMask;
				}
				else
				{
					m_oledMap[x][mapPage] &= ~mapMask;
				}

				//get next map row
				//- if end of glyph, go to next column
				y++;
				if ((y >= (y1 + height)) || (y >= OLED_PIX_HEIGHT))
				{
					break;
				}
			}
		}

		//end of glyph column, check if we're at end of map
		x++;
		if ((x >= (x1 + width)) || (x >= OLED_PIX_WIDTH))
		{
			break;
		}
	}
	xSemaphoreGive(m_oledMutex);
}

/******************************************************************************
* Oled_Refresh
*	Refresh display by drawing current memory map contents to display.
******************************************************************************/
inline void Oled_RefreshOrig (void)
{
	uint32_t	waiting = 0;
//	xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
//	printf("Oled_Refresh(): taking refresh semaphore.\n");
//	xSemaphoreGive(uart2_tx_buffer_mutexHandle);
	xSemaphoreTake(m_onRefresh, portMAX_DELAY);
	m_write_data();

	while (!m_transferComplete)
	{
		//oled_hard_delay_us(5);
		osDelay(1);
		if ((++waiting % 250000) == 0)
		{
			printf("m_WriteData(tp1): waiting...\n");
		}
	};

	/* Kluge to deal with premature xfer-complete */
	oled_hard_delay_us(10);
	LL_GPIO_SetOutputPin(SPI1_nSS_GPIO_Port, SPI1_nSS_Pin);

	xSemaphoreGive(m_onRefresh);
//	xSemaphoreTake(uart2_tx_buffer_mutexHandle, portMAX_DELAY);
//	printf("Oled_Refresh(): released refresh semaphore.\n");
//	xSemaphoreGive(uart2_tx_buffer_mutexHandle);
}

/******************************************************************************
* m_GetStringWidth
*	Private: Calculate width in pixels of string.
******************************************************************************/
static uint32_t m_GetStringWidth (const char *str)
{
	uint32_t width = 1;		//add 1 for space in front of first character
	while (*str)
	{
		//add width of character + 1 for space after character
		width += (Font5x7_CharWidth(*str) + 1);
		str++;
	}
	return (width);
}


/******************************************************************************
* Oled_PutChar
*	Draw a character at the specified line and column location.
*
* x:		x-coordinate reference
* y:		y-coordinate reference
* ch:		character to display
* flags:	see descriptions in .h
******************************************************************************/
void Oled_PutChar (uint32_t x, uint32_t y, uint8_t ch, uint32_t flags)
{
	Oled_DrawGlyphOrig(x, y, Font5x7_GetGlyph(ch), flags);
}

/******************************************************************************
* Oled_PutString
*	Draw a sequence of characters at the specified line and
*	column location. If requested, wrap the sequence to the next line if there is overlap.
*
* x:		x-coordinate reference
* y:		y-coordinate reference
* str:		string to display
* flags:	see descriptions in .h
******************************************************************************/
void Oled_PutStringOrig (uint32_t x, uint32_t y, const char *str, uint32_t flags)
{
	uint32_t width = m_GetStringWidth(str);
	bool fill = (flags & OLED_FLAG_INVERT) ? true : false;
	bool done = false;

	//check for horizontal center alignment
	if (flags & OLED_FLAG_ALIGN_HOR)
	{
		uint32_t offset = width >> 1;
		if (x >= offset)
		{
			x -= offset;
		}
	}
	//check for right alignment
	else if (flags & OLED_FLAG_ALIGN_RIGHT)
	{
		if (x >= width)
		{
			x -= width;
		}
	}

	//check for vertical center alignment
	if (flags & OLED_FLAG_ALIGN_VERT)
	{
		uint32_t offset = OLED_CHAR_HEIGHT >> 1;
		if (y >= offset)
		{
			y -= offset;
		}
	}
	else if (flags & OLED_FLAG_ALIGN_BOTTOM)
	{
		if (y >= OLED_CHAR_HEIGHT)
		{
			y -= OLED_CHAR_HEIGHT;
		}
	}

	//draw space before first character
	m_FillBox(x, y, 1, OLED_CHAR_HEIGHT, 0, fill);
	x++;

	while ((*str) && (!done))
	{
		width = Font5x7_CharWidth(*str);

		//if character fits then draw it
		if ((x + width) < OLED_PIX_WIDTH)
		{
			//clear other flags except invert
			uint32_t tempFlags = flags & OLED_FLAG_INVERT;

			//draw character
			Oled_DrawGlyphOrig(x, y, Font5x7_GetGlyph(*str), tempFlags);
			x += width;
			//draw space between characters
			m_FillBox(x, y, 1, OLED_CHAR_HEIGHT, 0, fill);
			x++;
			str++;
		}
		//if overlap, wrap to next line
		else if (flags & OLED_FLAG_WRAP_STRING)
		{
			//only overlap if there's another line to go to
			if (y >= OLED_LINE_HEIGHT)
			{
				y += OLED_LINE_HEIGHT;

				//draw space before first character
				m_FillBox(0, y, 1, OLED_CHAR_HEIGHT, 0, fill);
				x = 1;
			}
		}
		else
		{
			done = true;
		}
	}
}

/******************************************************************************
* m_FillBox
*	Draw or erase a rectangular section (box) of the memory map and display.
*
* x:		x-coordinate reference
* y:		y-coordinate reference
* width:	pixel width of box
* height:	pixel height of box
* fill:		0 = erase, 1 = fill
******************************************************************************/
static void m_FillBox (uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t flags, bool fill)
{
	//assign starting x-coord
	//- default is left alignment
	//- if aligning to center or right and there's not enough room, start at 0
	uint32_t x1 = x;
	if (flags & OLED_FLAG_ALIGN_HOR)
	{
		uint32_t offset = width >> 1;
		x1 = (x >= offset) ? (x - offset) : 0;
	}
	else if (flags & OLED_FLAG_ALIGN_RIGHT)
	{
		x1 = (x >= width) ? (x - width) : 0;
	}
	x = x1;

	//assign ending x-coord
	uint32_t x2 = x1 + width;
	if (x2 >= OLED_PIX_WIDTH)
	{
		x2 = OLED_PIX_WIDTH;
	}

	//assign starting y-coord
	//- default is top alignment
	//- if aligning to center or bottom and there's not enough room, start at 0
	uint32_t y1 = y;
	if (flags & OLED_FLAG_ALIGN_VERT)
	{
		uint32_t offset = height >> 1;
		y1 = (y >= offset) ? (y - offset) : 0;
	}
	else if (flags & OLED_FLAG_ALIGN_BOTTOM)
	{
		y1 = (y >= height) ? (y - height) : 0;
	}

	//assign ending y-coord
	uint32_t y2 = y1 + height;
	if (y2 >= OLED_PIX_HEIGHT)
	{
		y2 = OLED_PIX_HEIGHT;
	}

	//write data to memory map
	xSemaphoreTake(m_oledMutex, portMAX_DELAY);
	for (x = x1; x < x2; x++)
	{
		for (y = y1; y < y2; y++)
		{
			uint8_t mask = 1u << (y & 0x07u);
			uint32_t page = y >> 3;

			if (fill)
			{
				m_oledMap[x][page] |= mask;
			}
			else
			{
				m_oledMap[x][page] &= ~mask;
			}
		}
	}
	xSemaphoreGive(m_oledMutex);
}

/******************************************************************************
* Oled_DrawHline
*	Draw horizontal line.
*
* x:		x-coordinate reference
* y:		y-coordinate reference
* width:	pixel width of line
******************************************************************************/
void Oled_DrawHline (uint32_t x, uint32_t y, uint32_t width)
{
	m_FillBox(x, y, width, 1, 0, true);
}

void oled_DMA_SPI_TxCpltCallback(void)
{
	/* Called via DMA complete interrupt - raise SPI-nSS & flag xfer complete */
//	LL_GPIO_SetOutputPin(SPI1_nSS_GPIO_Port, SPI1_nSS_Pin);
	m_transferComplete = true;
	nr_transfer_complete_cbs++;
}


#pragma GCC  push_options
#pragma GCC  optimize ("O0")
static void oled_hard_delay_us (uint32_t usecs)
{
	// ToDo: Something better needed here
	uint32_t hard_delay = 20 * usecs; /* ~20 loops / us */
	while (hard_delay--)
	{
		m_dummyToFoolOptimization++;
	}

}

void start_dma_transfer (uint8_t buf[], uint32_t nr_bytes)
{
	/* Disable DMA prior to loading new params */
	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
	LL_DMA_ConfigAddresses(DMA2, /*LL_DMA_CHANNEL_2*/ LL_DMA_STREAM_2,
						   (uint32_t)buf,
						   LL_SPI_DMA_GetRegAddr(SPI1),
						   LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_SetDataLength(DMA2, /*LL_DMA_CHANNEL_2*/ LL_DMA_STREAM_2, nr_bytes);
	LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_2);
	LL_SPI_EnableDMAReq_TX(SPI1);
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);
}
#pragma GCC  pop_options

