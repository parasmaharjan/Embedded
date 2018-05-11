#ifndef __LCD_H__
#define __LCD_H__

#include <hidef.h>           /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <stdio.h>
#include <stdlib.h>
#include "delay.h"

	
	/******************************************************
	 * Change this according to your system configuration *
	 * LCD Configurations                                 *
	 ******************************************************/
	#define ROWS 2
	#define COLS 16
	
	#define LCD_DATA PORTK
	#define LCD_CTRL PORTK
	#define RS 0x01
	#define EN 0x02
	/******************************************************/
	
		
	// LCD commands
	#define LCD_CLEARDISPLAY 0x01
	#define LCD_RETURNHOME 0x02
	#define LCD_ENTRYMODESET 0x04
	#define LCD_DISPLAYCONTROL 0x08
	#define LCD_CURSORSHIFT 0x10
	#define LCD_FUNCTIONSET 0x20
	#define LCD_SETCGRAMADDR 0x40
	#define LCD_SETDDRAMADDR 0x80

	// flags for display entry mode
	#define LCD_ENTRYRIGHT 0x00
	#define LCD_ENTRYLEFT 0x02
	#define LCD_ENTRYSHIFTINCREMENT 0x01
	#define LCD_ENTRYSHIFTDECREMENT 0x00

	// flags for display on/off control
	#define LCD_DISPLAYON 0x04
	#define LCD_DISPLAYOFF 0x00
	#define LCD_CURSORON 0x02
	#define LCD_CURSOROFF 0x00
	#define LCD_BLINKON 0x01
	#define LCD_BLINKOFF 0x00

	// flags for display/cursor shift
	#define LCD_DISPLAYMOVE 0x08
	#define LCD_CURSORMOVE 0x00
	#define LCD_MOVERIGHT 0x04
	#define LCD_MOVELEFT 0x00

	// flags for function set
	#define LCD_8BITMODE 0x10
	#define LCD_4BITMODE 0x00
	#define LCD_2LINE 0x08
	#define LCD_1LINE 0x00
	#define LCD_5x10DOTS 0x04
	#define LCD_5x8DOTS 0x00
	
	

	
	void lcd_init(void);
	void lcd_clear(void);
	void lcd_home(void);
	void lcd_set_cursor(unsigned int col, unsigned int row);	
	void lcd_send_cmd(unsigned char);
	void lcd_send_char(unsigned char);
	void lcd_send_str(unsigned char * str);
	void lcd_set_row_offsets(unsigned int row0,unsigned int row1,unsigned int row2,unsigned int row3);
	void lcd_custom_char(byte * custom_char, int num);
#endif
