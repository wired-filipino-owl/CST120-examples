/*
 * wave_counter_rgb.c
 *
 * Created: 6/10/2023 2:57:50 PM
 * Author : Rob
 */ 
#define F_CPU 16000000

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

bool start = true;
bool led_on = false;
uint16_t start_time = 0;
uint16_t pulse_time = 0;
uint16_t frequency = 0;

int main(void)
{
	DDRB |= (1 << RED_PIN) | (1 << GREEN_PIN) | (1 << BLUE_PIN) | (1 << EXT_LED);	//Set our RGB LED and onboard LED pins to outputs
	DDRD &= (0 << TRIG_PIN);						//set our trigger pin as an input
	
	TCCR1B = (1<<CS12) | (0<<CS11) | (0<<CS10);		//set Prescaler of Timer1 (16-bit) to 256 (62500 counts per second) (section 15.11.2)
	TIMSK1 = (1 << OCIE1A);							//Interrupt on output compare match
	OCR1A = 62500 / 2;								//Interrupt every 500ms
	
	TCNT1 = 0;										//Clear Timer1's count
	
	EICRA = (1<<ISC11) | (0<<ISC10);				//Interrupt on falling edge (section 12.2)
	EIMSK = (1<<INT0);								//Enable external interrupt 0 (section 12.2.2)
	sei();											//DO THIS LAST! Enable global interrupts
	
	//output test
	PORTB |= (1 << RED_PIN);
	_delay_ms(500);
	PORTB &= (0 << RED_PIN);
	PORTB |= (1 << BLUE_PIN);
	_delay_ms(500);
	PORTB &= (0 << BLUE_PIN);
	PORTB |= (1 << GREEN_PIN);
	_delay_ms(500);
	PORTB &= (0 << GREEN_PIN);
	
    while (1) 
    {
		frequency = ( 1 / (pulse_time / 62500) ); //frequency = 1 / time	
		
		if(frequency >= 200 && frequency <= 400)
		{
			//red
			PORTB |= (1 << RED_PIN);
			PORTB &= (0 << GREEN_PIN) | (0 << BLUE_PIN);
		}
		else if(frequency >= 400 && frequency <= 600)
		{
			//green
			PORTB |= (1 << GREEN_PIN);
			PORTB &= (0 << RED_PIN) | (0 << BLUE_PIN);
		}
		else if(frequency >= 600 && frequency <= 800)
		{
			//blue
			PORTB |= (1 << BLUE_PIN);
			PORTB &= (0 << RED_PIN) | (0 << GREEN_PIN);
		}
		else
		{
			//off
			//PORTB &= (0 << RED_PIN)	| (0 << GREEN_PIN) | (0 << BLUE_PIN);
		}
		
		//wait 1 second
		//_delay_ms(1000);
    }
}

//interrupt when we receive a falling edge
ISR(INT0_vect)
{
	if (start)
	{
		start_time = TCNT1;
		start = false;
	}
	else
	{
		pulse_time = TCNT1 - start_time;
		start = true;
	}
}

ISR(TIMER1_COMPA_vect)
{
	//blink LED every 500ms
	if (led_on)
	{
		DDRB &= (0 << EXT_LED);
		led_on = false;
	}
	else
	{
		DDRB |= (1 << EXT_LED);
		led_on = true;
	}
}
