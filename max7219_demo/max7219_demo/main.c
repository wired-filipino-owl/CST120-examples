/*
 * max7219_demo.c
 *
 * Created: 5/26/2023 10:32:18 AM
 * Author : Rob Galeoto
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "MAX7219.h"

#define MAX7219_SETUP
#define MOSI_DD		DDB3	//digital pin 11
#define SCK_DD		DDB5	//digital pin 13
#define CS_DD		DDB2	//digital pin 10

#define CS			PINB2

#define SCROLL_DELAY 250.0

const uint8_t letter_r[8] =
{
	0b10000001,
	0b10111101,
	0b10111101,
	0b10110011,
	0b10000111,
	0b10111011,
	0b10111011,
	0b10111101
};

const uint8_t letter_o[8] =
{
	0b11000011,
	0b10111101,
	0b10111101,
	0b10111101,
	0b10111101,
	0b10111101,
	0b10111101,
	0b11000011	
};

const uint8_t letter_b[8] = 
{
	0b10000011,
	0b10111101,
	0b10111101,
	0b10000111,
	0b10111011,
	0b10111101,
	0b10111101,
	0b10000001
};

const uint8_t exc_mark[8] =
{
	0b11100111,
	0b11100111,
	0b11100111,
	0b11100111,
	0b11100111,
	0b11111111,
	0b11100111,
	0b11100111
};

const uint8_t q_mark[8] =
{
	0b11000111,
	0b10111011,
	0b11111011,
	0b11110111,
	0b11101111,
	0b11101111,
	0b11111111,	
	0b11101111
};

const uint8_t sm_face[8] =
{
	0b11011011,
	0b11011011,
	0b11011011,
	0b11011011,
	0b01111110,
	0b10111101,
	0b11011011,
	0b11100111	
};

int main(void)
{
	//Setup the display
	SetupSPI();
	
	EnableAllRows();
	_delay_us(10);
	ExitShutdownMode();
	_delay_us(10);
	ClearDisplay();
	_delay_us(10);
	
    while (1) 
    {
		//basic fill patterns
		for (int i = 1; i < 9; i++)
		{
			PORTB &= (0 << CS);
			SendByte(i);
			SendByte(0xFF);
			PORTB |= (1 << CS);
			_delay_ms(500);
		}		
		
		for (int j = 1; j < 9; j++)
		{
			PORTB &= (0 << CS);
			SendByte(j);
			SendByte(0x00);
			PORTB |= (1 << CS);
			_delay_ms(500);
		}
		
		uint8_t col = 128;
		while (col > 0)
		{
			for (int row = 1; row < 9; row++)
			{
				PORTB &= (0 << CS);
				SendByte(row);
				SendByte(col);
				PORTB |= (1 << CS);
			}
			col = col >> 1;
			_delay_ms(500);
		}
		
		col = 128;
		while (col > 0)
		{
			for (int row = 1; row < 9; row++)
			{
				PORTB &= (0 << CS);
				SendByte(row);
				SendByte(~col);
				PORTB |= (1 << CS);
			}
			col = col >> 1;
			_delay_ms(500);
		}
		
		//demonstrate drawing to the whole display at once
		DrawCharacter(letter_r);
		_delay_ms(750);		
		DrawCharacterInverted(letter_r);
		_delay_ms(750);
		DrawCharacter(letter_o);
		_delay_ms(750);
		DrawCharacterInverted(letter_o);
		_delay_ms(750);		
		DrawCharacter(letter_b);
		_delay_ms(750);
		DrawCharacterInverted(letter_b);
		_delay_ms(750);		
		DrawCharacter(exc_mark);
		_delay_ms(750);
		DrawCharacterInverted(exc_mark);
		_delay_ms(750);		
		DrawCharacter(q_mark);
		_delay_ms(750);
		DrawCharacterInverted(q_mark);
		_delay_ms(750);		
		DrawCharacter(sm_face);
		_delay_ms(750);
		DrawCharacterInverted(sm_face);
		_delay_ms(750);			
		
		
		//Scroll demo
		ScrollCharacterRight(exc_mark);
		_delay_ms(750);
		
		ScrollCharacterLeft(exc_mark);
		_delay_ms(750);
			
		ScrollCharacterRightInverted(exc_mark);
		_delay_ms(750);
		
		ScrollCharacterLeftInverted(exc_mark);
		_delay_ms(750);
						
		ClearDisplay();
		_delay_ms(500);
    }
}
