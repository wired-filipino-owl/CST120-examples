/*
 * MAX7219.h
 *
 * Created: 5/26/2023 8:29:20 PM
 *  Author: Rob
 */ 


#ifndef MAX7219_H_
#define MAX7219_H_

#define F_CPU 16000000UL
//defaults for ATMEGA328P
//want to change? Define MAX7219 setup in your main.c
#ifndef MAX7219_SETUP
	#define MOSI_DD		DDB3	//digital pin 11
	#define SCK_DD		DDB5	//digital pin 13
	#define CS_DD		DDB2	//digital pin 10
	#define CS			PINB2
	#define SCROLL_DELAY 250.0
#endif

#include <avr/io.h>
#include <util/delay.h>

void SetupSPI();
void SendByte(uint8_t byte);
void ExitShutdownMode();
void DisplayTest();
void ClearDisplay();
void EnableAllRows();
void DrawCharacter(const uint8_t character[8]);
void DrawCharacterInverted(const uint8_t character[8]);
void ScrollCharacterRight(const uint8_t character[8]);
void ScrollCharacterLeft(const uint8_t character[8]);
void ScrollCharacterRightInverted(const uint8_t character[8]);
void ScrollCharacterLeftInverted(const uint8_t character[8]);

#endif /* MAX7219_H_ */
