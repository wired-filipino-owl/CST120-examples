/*
 * lcd.h
 *
 * Created: 2020
 *  Authors: drouantg & deyr
 *  Revised for 4-bit operation by Rob Galeoto in 2024
 */ 
#undef  F_CPU
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define LCD_data_dir		DDRD
#define LCD_command_dir	DDRB
#define LCD_data_port	PORTD	//PORTD defined as DATA Port -> sends characters to LCD
#define LCD_command_port	PORTB	//PORTB defined as Port used to send Commands to LCD
#define RS	PB0	//Port B bit 0 defined as register select bit
//#define RW	PB1	//Port B bit 1 defined as Read/Write' bit //-----RW is permanently tied to ground on the LCD shields
#define EN	PB1	//Port B bit 1 is defined as Enable bit
#define Backlight_EN PB2

void LCD_Init(void);
void LCD_send_enable(void);
void LCD_command (char cmnd);
void LCD_char(char char_data);
void LCD_string (char *str);
void LCD_clear();