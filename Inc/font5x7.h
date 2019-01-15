/*
 * font5x7.h
 *
 * Definitions for 5x7 character font
 *
 * Created: 8/26/2013
 *  Author: chris.parker
 */ 

#ifndef FONT5X7_H_INCLUDED
#define FONT5X7_H_INCLUDED

//#include <asf.h>


/*
*	DEFINITIONS
**********************************************************/
#define FONT5X7_CHAR_HEIGHT			(9)		//pixel height of font
#define FONT5X7_CHAR_WIDTH			(5)		//pixel width of widest character

//character map/table defines
#define FONT5X7_TABLE_SIZE			(256)	//num entries in table
#define FONT5X7_TABLE_DATA_SIZE		(12)	//num bytes for each table entry
#define FONT5X7_WIDTH_BYTE			(0)		//offset of character width byte
#define FONT5X7_HEIGHT_BYTE			(1)		//offset of character height byte
#define FONT5X7_DATA_START			(2)		//offset of start of bitmap data

//character map/table offsets for non-standard ascii symbols
#define FONT5X7_UP_TRIANGLE			(16)
#define FONT5X7_DN_TRIANGLE			(17)
#define FONT5X7_LT_TRIANGLE			(18)
#define FONT5X7_RT_TRIANGLE			(19)
#define FONT5X7_UP_DN_TRIANGLES		(20)
#define FONT5X7_CURSOR				(21)
#define FONT5X7_INFO				(22)
#define FONT5X7_BINARY_OFF			(23)
#define FONT5X7_BINARY_ON			(24)
//
#define FONT5X7_BULLET				(149)
#define FONT5X7_EN_DASH				(150)
#define FONT5X7_EM_DASH				(151)
//
#define FONT5X7_DEGREE_SIGN			(176)

/*
*			PROTOTYPES
**********************************************************/
const uint8_t* Font5x7_GetGlyph (uint8_t ch);

uint32_t Font5x7_CharWidth (uint8_t ch);
uint32_t Font5x7_StringWidth (const uint8_t *str);


#endif /* FONT5X7_H_INCLUDED */
