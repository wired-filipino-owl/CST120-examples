/*
 * wave_counter_rgb.c
 *
 * Created: 6/10/2023 2:57:50 PM
 * Author : Rob Galeoto
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

//pinout for Uno Board:
//Red: Digital Pin 9
//Green: Digital Pin 10
//Blue: Digital Pin 11
//Trigger: Digital Pin 2

//RGB LED
#define RED_PIN PINB1
#define GREEN_PIN PINB2
#define BLUE_PIN PINB3
//onboard LED
#define EXT_LED PINB5
//Trigger Pin
#define TRIG_PIN PIND2

//variables used for timing the input from the Trigger Pin
bool start = true;
uint16_t start_time = 0;
uint16_t end_time = 0;
uint16_t check = 0;

int main(void)
{
	DDRB |= (1 << RED_PIN) | (1 << GREEN_PIN) | (1 << BLUE_PIN) | (1 << EXT_LED);	//Set our RGB LED and onboard LED pins to outputs
	DDRD &= (0 << TRIG_PIN);						//set our trigger pin as an input
	
	TCCR1B = (1<<CS12) | (0<<CS11) | (0<<CS10);		//set Prescaler of Timer1 (16-bit) to 256 (62500 counts per second) (section 15.11.2)
	TIMSK1 = (1 << OCIE1A);							//Interrupt on output compare match
	OCR1A = 62500 / 2;								//Interrupt every 500ms
	
	TCNT1 = 0;										//Clear Timer1's count
	
	//external interrupt 0
	EICRA = (1<<ISC01) | (1<<ISC00);				//Interrupt on rising edge (section 12.2)
	EIMSK = (1<<INT0);								//Enable external interrupt 0 (section 12.2.2)
	
	sei();											//DO THIS LAST! Enable global interrupts
	
	//output test
	PORTB |= (1 << RED_PIN);
	_delay_ms(1000);
	PORTB &= (0 << RED_PIN);
	PORTB |= (1 << BLUE_PIN);
	_delay_ms(1000);
	PORTB &= (0 << BLUE_PIN);
	PORTB |= (1 << GREEN_PIN);
	_delay_ms(1000);
	PORTB &= (0 << GREEN_PIN);
	
    while (1) 
    {
		check = end_time - start_time;	// this is the period of the signal
		
		//how to calculate "check" value: (1 / frequency) * 62500
		
		//if(frequency >= 200.0F && frequency <= 400.0F)
		if(check >= 155 && check <= 315)
		{
			//red
			PORTB |= (1 << RED_PIN);
			PORTB &= (0 << GREEN_PIN) | (0 << BLUE_PIN);
		}
		//else if(frequency >= 400.0F && frequency <= 600.0F)
		else if(check >= 105 && check < 155)
		{
			//green
			PORTB |= (1 << GREEN_PIN);
			PORTB &= (0 << RED_PIN) | (0 << BLUE_PIN);
		}
		//else if(frequency >= 600.0F && frequency <= 800.0F)
		else if(check >= 80 && check < 105)
		{
			//blue
			PORTB |= (1 << BLUE_PIN);
			PORTB &= (0 << RED_PIN) | (0 << GREEN_PIN);
		}
		else if (check == 0)
		{
			//purple
			PORTB |= (1 << RED_PIN) | (1 << BLUE_PIN);
		}
		else
		{
			//off
			PORTB &= (0 << RED_PIN) | (0 << GREEN_PIN) | (0 << BLUE_PIN);
		}
    }
}

//interrupt when we receive a rising edge
ISR(INT0_vect)
{
	if (start)
	{
		start_time = TCNT1;
		start = false;
	}
	else
	{
		end_time = TCNT1;
		start = true;
	}
}

ISR(TIMER1_COMPA_vect)
{
	//blink LED every 500ms
	DDRB ^= (1 << EXT_LED);
}
