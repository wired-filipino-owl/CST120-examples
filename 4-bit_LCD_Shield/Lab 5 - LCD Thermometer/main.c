/*
 * main.c
 *
 * Created: 5/8/2024 1:23:52 PM
 *  Authors: Rob Galeoto, George Drouant, "deyr"
 */ 

//#define F_CPU 16000000UL

#include "lcd.h"

int main(void)
{
	
    LCD_Init();//clears display and sets display to first char position, first row
	//LCD_string("Embedded System");//print string constant to LCD
	//LCD_command(0xC0);//go to first char of the 2nd line of the display
	//LCD_string("Laboratory");//print string constant to LCD    
	
    while (1) 
    {
		LCD_command_port |= 1 << Backlight_EN;	//Turn on the backlight
		_delay_ms(750);
		LCD_command_port &= ~(1 << Backlight_EN);	//Turn on the backlight
		_delay_ms(750);
    }
	
	return 0;
}
