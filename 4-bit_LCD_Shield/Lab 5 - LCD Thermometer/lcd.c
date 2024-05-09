/*
 * lcd.c
 *
 * Created: 2020
 *  Authors: drouantg & deyr
 *  Revised for 4-bit operation by Rob Galeoto in 2024
 */ 

#include "lcd.h"

void LCD_Init(void)
{
	/* Initialize LCD */
	LCD_command_dir = 0x7 << DDB0;			//PB0, PB1, PB2 set as output pins
	LCD_data_dir = 0xF << DDD4;				//Upper 4 pins of Port D are outputs
	LCD_command_port = 1 << Backlight_EN;	//Turn on the backlight
	_delay_ms(150);							// wait for internal initialization of LCD
	
	LCD_command_port &= ~(1<<EN | 1<<RS);	//sets RS bit Low -> Instruction Register, set enable low
	
	//Attempt to initialize HD44780 to 4-bit operation
	LCD_data_port = 0x3 << 4;
	LCD_send_enable();
	_delay_us(4500);	
	LCD_data_port = 0x3 << 4;
	LCD_send_enable();
	_delay_us(4500);
	LCD_data_port = 0x3 << 4;
	LCD_send_enable();
	_delay_us(150);
	LCD_data_port = 0x2 << 4;
	LCD_send_enable();
	_delay_us(150);
	
	LCD_command(0x28);	//sets 4-bit interface with LCD, 2 Line Display,5x8 dot character font
	LCD_command(0x04);	//turns off display
	LCD_command(0x01);	//clears entire Display
	LCD_command(0x0C);	//turns Display On with Cursor Off
	LCD_command(0x06);	//sets increment character position by one
	LCD_command(0x02);	//Sets Display to start at first character of first Line
	_delay_ms(3);
}

void LCD_send_enable(void)
{
	LCD_command_port &= ~(1<<EN);
	_delay_us(1);
	LCD_command_port |= (1<<EN);
	_delay_us(1);
	LCD_command_port &= ~(1<<EN);
	_delay_us(50);
}

void LCD_command (char cmnd)
{
	/*write a command to LCD */
	LCD_command_port &= ~(1<<RS | 1<<EN);	//sets RS bit Low -> Instruction Register
	LCD_data_port = cmnd & 0xF0;			//outputs high nibble of hex command code to PORTD and the LCD’s data bus
	LCD_send_enable();
	LCD_data_port = (cmnd & 0x0F) << 4;		//outputs low nibble of hex command code to PORTD and the LCD’s data bus
	LCD_send_enable();
	_delay_ms(2);							//give LCD time to complete write
}

void LCD_char(char char_data)
{
	/* write a character to LCD */
	LCD_command_port |= (1<<RS);			//sets RS bit High -> Data Register
	LCD_command_port &= ~(1<<EN);			//sets EN bit low
	LCD_data_port = char_data & 0xF0;		//outputs char to PORTD and the data bus of the LCD
	LCD_send_enable();
	LCD_data_port = char_data << 4;			//outputs char to PORTD and the data bus of the LCD
	LCD_send_enable();
	_delay_ms(2);							//give LCD time to complete write
}

void LCD_string (char *str)
{
	/* Walks through a string a character at a time until
	finding a null terminator at the end of the string. Sends a character
     at a time to to function LCD_char */
	int i;
	for(i=0; str[i]!= 0; i++)
	{
		LCD_char(str[i]);
	}
}


void LCD_clear()//clears display and goes to position of 1st char on 1st line
{
	LCD_command (0x01);//clears entire Display
	_delay_ms(3);
}