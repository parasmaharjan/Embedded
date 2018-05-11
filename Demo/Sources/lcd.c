#include "lcd.h"


unsigned int _row_offsets[4];
unsigned int _numlines;

/* Initialization function for LCD */
void lcd_init(void)
{
	_numlines = COLS;
	lcd_set_row_offsets(0x00, 0x40, 0x00 + COLS, 0x40 + COLS);
	
	/* Set PORT K as output */
	DDRK = 0xFF;
	
	lcd_send_cmd(0x33);   //reset sequence provided by data sheet
	delay_ms(1);
	lcd_send_cmd(0x32);   //reset sequence provided by data sheet
	delay_ms(1);
	lcd_send_cmd(0x28);   //Function set to four bit data length
	                  //2 line, 5 x 7 dot format
	delay_ms(1);
	lcd_send_cmd(0x06);  //entry mode set, increment, no shift
	delay_ms(1);
	lcd_send_cmd(0x0C);  //Display set, disp on, cursor on, blink off
	delay_ms(1);
	lcd_send_cmd(0x01);  //Clear display
	delay_ms(1);
	lcd_send_cmd(0x80);  //set start posistion, home position
	delay_ms(1);
}


/* Set offset for rows */
void lcd_set_row_offsets(unsigned int row0,unsigned int row1,unsigned int row2,unsigned int row3)
{
	_row_offsets[0] = row0;
	_row_offsets[1] = row1;
	_row_offsets[2] = row2;
	_row_offsets[3] = row3;
}


/* Clear lcd screen */
void lcd_clear(void) 
{
	lcd_send_cmd(LCD_CLEARDISPLAY);
	delay_ms(1);
}


/* Clear lcd screen */
void lcd_home(void) 
{
	lcd_send_cmd(LCD_RETURNHOME);
	delay_ms(1);
}


/* Set the LCD cursor to desired location in screen */
void lcd_set_cursor(unsigned int col, unsigned int row)
{
	const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
	if ( row >= max_lines ) 
	{
		row = max_lines - 1;    // we count rows starting w/0
	}
	if ( row >= _numlines ) 
	{
		row = _numlines - 1;    // we count rows starting w/0
	}

	lcd_send_cmd(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}


/* Send command to lcd */
void lcd_send_cmd(unsigned char command)
{
	unsigned char x;

	x = (command & 0xF0) >> 2;         //shift high nibble to center of byte for Pk5-Pk2
	LCD_DATA =LCD_DATA & ~0x3C;        //clear bits Pk5-Pk2
	LCD_DATA = LCD_DATA | x;           //sends high nibble to PORTK
	delay_ms(1);
	LCD_CTRL = LCD_CTRL & ~RS;         //set RS to command (RS=0)
	delay_ms(1);
	LCD_CTRL = LCD_CTRL | EN;          //rais enable
	delay_ms(1);
	LCD_CTRL = LCD_CTRL & ~EN;         //Drop enable to capture command
	delay_ms(1);                       //wait
	
	x = (command & 0x0F)<< 2;          // shift low nibble to center of byte for Pk5-Pk2
	LCD_DATA =LCD_DATA & ~0x3C;         //clear bits Pk5-Pk2
	LCD_DATA =LCD_DATA | x;             //send low nibble to PORTK
	LCD_CTRL = LCD_CTRL | EN;          //rais enable
	delay_ms(1);
	LCD_CTRL = LCD_CTRL & ~EN;         //drop enable to capture command
	delay_ms(1);
}


/* Send character data to lcd */
void lcd_send_char(unsigned char data)
{
	unsigned char x;

	x = (data & 0xF0) >> 2;
	LCD_DATA =LCD_DATA & ~0x3C;                     
	LCD_DATA = LCD_DATA | x;
	delay_ms(1);
	LCD_CTRL = LCD_CTRL | RS;
	delay_ms(1);
	LCD_CTRL = LCD_CTRL | EN;
	delay_ms(1);
	LCD_CTRL = LCD_CTRL & ~EN;
	delay_ms(1);

	x = (data & 0x0F)<< 2;
	LCD_DATA =LCD_DATA & ~0x3C;                     
	LCD_DATA = LCD_DATA | x;
	LCD_CTRL = LCD_CTRL | EN;
	delay_ms(1);
	LCD_CTRL = LCD_CTRL & ~EN;
	delay_ms(1);
}


/* Send continous string to lcd */
void lcd_send_str(char * str)
{
	while(*str)
	{
		lcd_send_char(*str);
		str++;
	}
}

void lcd_custom_char(byte * custom_char, int num)
{
	int j;
	
	lcd_send_cmd(0x40);
	for(j = 0 ; j < (num*8) ; j++)
	{
		lcd_send_char(custom_char[j]);
	}
}







