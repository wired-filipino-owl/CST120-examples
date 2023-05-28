/*
 * MAX7219.c
 *
 * Created: 5/26/2023 8:28:46 PM
 *  Author: Rob Galeoto
 */ 

#include "MAX7219.h"

uint8_t current_char[8] = {};
bool char_filled = false;

void SetupSPI()
{
	//set MOSI, SCK, and CS as outputs
	DDRB |= (1<<MOSI_DD)|(1<<SCK_DD)|(1<<CS_DD);
	/* Enable SPI, Master, set clock rate fck/2 (SPI Clock 8 MHz)*/
	SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPR1)|(0<<SPR0);
}

void SendByte(uint8_t byte)
{
	SPDR = byte;
	//wait for transfer to complete
	while(!(SPSR & (1<<SPIF)));
}

void ExitShutdownMode()
{
	PORTB &= (0 << CS);
	SendByte(0x0C);
	SendByte(0xFF);
	PORTB |= (1 << CS);
}

void DisplayTest()
{
	PORTB &= (0 << CS);
	SendByte(0x0F);
	SendByte(0xFF);
	PORTB |= (1 << CS);
}

void ClearDisplay()
{
	for (int i = 1; i < 9; i++)
	{
		PORTB &= (0 << CS);
		SendByte(i);
		SendByte(0x00);
		PORTB |= (1 << CS);
		//allow time for the MAX7219 to latch the data
		_delay_us(10);
	}
}

void EnableAllRows()
{
	PORTB &= (0 << CS);
	SendByte(0x0B);
	SendByte(0x07);
	PORTB |= (1 << CS);
}

void DrawCharacter(const uint8_t character[8])
{
	for (int row = 1; row < 9; row++)
	{
		PORTB &= (0 << CS);
		SendByte(row);
		SendByte(character[row-1]);
		PORTB |= (1 << CS);
		_delay_us(10);
	}
}

void DrawCharacterInverted(const uint8_t character[8])
{
	for (int row = 1; row < 9; row++)
	{
		PORTB &= (0 << CS);
		SendByte(row);
		SendByte(~character[row-1]);
		PORTB |= (1 << CS);
		_delay_us(10);
	}
}

void ScrollCharacterRight(const uint8_t character[8])
{
	//start all the way left
	for (int scroll = 7; scroll > 0; scroll--)
	{
		for (int row = 1; row < 9; row++)
		{
			PORTB &= (0 << CS);
			SendByte(row);
			SendByte(character[row-1] << scroll);
			PORTB |= (1 << CS);
			_delay_us(10);
		}
		
		_delay_ms(SCROLL_DELAY);
	}
	//scroll off screen
	for (int scroll = 0; scroll < 9; scroll++)
	{
		for (int row = 1; row < 9; row++)
		{
			PORTB &= (0 << CS);
			SendByte(row);
			SendByte(character[row-1] >> scroll);
			PORTB |= (1 << CS);
			_delay_us(10);
		}
		
		_delay_ms(SCROLL_DELAY);
	}
}

void ScrollCharacterLeft(const uint8_t character[8])
{
	//start all the way right
	for (int scroll = 7; scroll > 0; scroll--)
	{
		for (int row = 1; row < 9; row++)
		{
			PORTB &= (0 << CS);
			SendByte(row);
			SendByte(character[row-1] >> scroll);
			PORTB |= (1 << CS);
			_delay_us(10);
		}
		
		_delay_ms(SCROLL_DELAY);
	}
	
	for (int scroll = 0; scroll < 9; scroll++)
	{
		for (int row = 1; row < 9; row++)
		{
			PORTB &= (0 << CS);
			SendByte(row);
			SendByte(character[row-1] << scroll);
			PORTB |= (1 << CS);
			_delay_us(10);
		}
		
		_delay_ms(SCROLL_DELAY);
	}
}

void ScrollCharacterRightInverted(const uint8_t character[8])
{
	//start all the way left
	for (int scroll = 7; scroll > 0; scroll--)
	{
		for (int row = 1; row < 9; row++)
		{
			PORTB &= (0 << CS);
			SendByte(row);
			SendByte(~character[row-1] << scroll);
			PORTB |= (1 << CS);
			_delay_us(10);
		}
		
		_delay_ms(SCROLL_DELAY);
	}
	//scroll off screen
	for (int scroll = 0; scroll < 9; scroll++)
	{
		for (int row = 1; row < 9; row++)
		{
			PORTB &= (0 << CS);
			SendByte(row);
			SendByte(~character[row-1] >> scroll);
			PORTB |= (1 << CS);
			_delay_us(10);
		}
		
		_delay_ms(SCROLL_DELAY);
	}
}

void ScrollCharacterLeftInverted(const uint8_t character[8])
{
	//start all the way right
	for (int scroll = 7; scroll > 0; scroll--)
	{
		for (int row = 1; row < 9; row++)
		{
			PORTB &= (0 << CS);
			SendByte(row);
			SendByte(~character[row-1] >> scroll);
			PORTB |= (1 << CS);
			_delay_us(10);
		}
		
		_delay_ms(SCROLL_DELAY);
	}
	
	for (int scroll = 0; scroll < 9; scroll++)
	{
		for (int row = 1; row < 9; row++)
		{
			PORTB &= (0 << CS);
			SendByte(row);
			SendByte(~character[row-1] << scroll);
			PORTB |= (1 << CS);
			_delay_us(10);
		}
		
		_delay_ms(SCROLL_DELAY);
	}
}

void ScrollContinuous(const uint8_t first[8], const uint8_t second[8])
{
	for (int shift = 0; shift < 9; shift++)
	{
		for (int row = 1; row < 9; row++)
		{
			PORTB &= (0 << CS);
			SendByte(row);
			SendByte( (first[row-1] << shift) | (second[row-1] >> (8 - shift) ) );
			PORTB |= (1 << CS);
			_delay_us(10);			
		}
		_delay_ms(SCROLL_DELAY);
	}
}

void ScrollInLeft(uint8_t * character)
{
	if(!char_filled)
	{
		memset(current_char, 0, sizeof(uint8_t[8]) );
	}
	
	for (int shift = 0; shift < 9; shift++)
	{
		for (int row = 1; row < 9; row++)
		{
			PORTB &= (0 << CS);
			SendByte(row);
			SendByte( (current_char[row-1] << shift) | (character[row-1] >> (8 - shift) ) );
			PORTB |= (1 << CS);
			_delay_us(10);
		}
		_delay_ms(SCROLL_DELAY);
	}
	
	memcpy((void*) &current_char, (const void *) character, sizeof(uint8_t[8]));
	char_filled = true;
}

void ScrollInRight(uint8_t * character)
{
	if(!char_filled)
	{
		memset(current_char, 0, sizeof(uint8_t[8]) );
	}
	
	for (int shift = 0; shift < 9; shift++)
	{
		for (int row = 1; row < 9; row++)
		{
			PORTB &= (0 << CS);
			SendByte(row);
			SendByte( (current_char[row-1] >> shift ) | (character[row-1] << (8 - shift) ) );
			PORTB |= (1 << CS);
			_delay_us(10);
		}
		_delay_ms(SCROLL_DELAY);
	}
	
	memcpy((void*) &current_char, (const void *) character, sizeof(uint8_t[8]));
	char_filled = true;
}

void ClearScrollChar()
{
	memset(current_char, 0, sizeof(uint8_t[8]) );
	char_filled = false;
}
