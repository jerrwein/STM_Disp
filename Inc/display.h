/*
 * display.h
 *
 *  Created on: Jan 10, 2019
 *      Author: jweinzierl
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

typedef enum
{
	DISPLAY_MODE_NONE,
	DISPLAY_MODE_MENU,
	DISPLAY_MODE_CHARGING,
	DISPLAY_MODE_ESTOP,
	DISPLAY_MODE_RTF,
	DISPLAY_MODE_PURGE,
	DISPLAY_MODE_SCRUB
} display_mode_t;

//text lines for screens using header and footer
typedef enum
{
	DISPLAY_LINE_1,
	DISPLAY_LINE_2,
	DISPLAY_LINE_3,
	DISPLAY_LINE_4,
	//
	DISPLAY_LINES_PER_SCREEN

} display_line_t;

//header/footer/line positions (position sets top pixel)
#define DISPLAY_HEADER_Y		(1u)
#define DISPLAY_HEADER_LINE_Y	(11u)
#define DISPLAY_LINE_1_Y		(13u)
#define DISPLAY_LINE_2_Y		(23u)
#define DISPLAY_LINE_3_Y		(33u)
#define DISPLAY_LINE_4_Y		(43u)
#define DISPLAY_FOOTER_LINE_Y	(53u)
#define DISPLAY_FOOTER_Y		(55u)

//left/right edge of text lines
//- leave room on left for cursor
#define	DISPLAY_LINE_LEFT_X		(7u)
#define	DISPLAY_LINE_RIGHT_X	(127u)

//display flags
#define DISPLAY_FLAG_INDENT		(1u << 0)	//indent for line cursor
#define DISPLAY_FLAG_INVERT		(1u << 1)	//invert pixels

//footer types
typedef enum
{
	DISPLAY_FOOTER_BACK,
	DISPLAY_FOOTER_BACK_ADD,
	DISPLAY_FOOTER_BACK_DELETE,
	DISPLAY_FOOTER_BACK_MAKESUPERVISOR,
	DISPLAY_FOOTER_BACK_SCROLL,
	DISPLAY_FOOTER_BACK_SCROLL_PAGE,
	DISPLAY_FOOTER_BACK_SELECT,
	DISPLAY_FOOTER_BACK_SELECT_SAVE,
	DISPLAY_FOOTER_BACK_SET,
	DISPLAY_FOOTER_CANCEL,
	DISPLAY_FOOTER_CANCEL_CLEAR,
	DISPLAY_FOOTER_CANCEL_EDIT_SAVE,
	DISPLAY_FOOTER_CANCEL_INSTALL,
	DISPLAY_FOOTER_CANCEL_RESET,
	DISPLAY_FOOTER_CANCEL_SAVE,
	DISPLAY_FOOTER_EXIT_SELECT,

} display_footer_t;


uint32_t display_init (void);
void run_display_task (void const * argument);
void Display_DrawHeader (const char *title, uint32_t level);
void Display_DrawFooter (display_footer_t type);
void Display_DrawLineCursor (display_line_t line);
void Display_DrawLineLabel (display_line_t line, const char *labelStr, uint32_t flags);
void Display_DrawMenuXofY (uint32_t item, uint32_t total);

#endif /* DISPLAY_H_ */
