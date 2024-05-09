/*
 * main.c
 *
 * Created: 5/8/2024 1:23:52 PM
 *  Authors: Rob Galeoto, George Drouant, "deyr"
 */ 

//#define F_CPU 16000000UL

#include <stdio.h>
#include <math.h>
#include "lcd.h"

uint16_t ADC_data = 0;
float ADC_voltage = 0.0;
float Temperature = 0.0;
char result[10]  = "";

int main(void)
{	
	ADMUX = (1 << REFS0 | 1 << MUX0);	//AVcc (5V supply) used for Vref; Analog Channel 1 (ADC1) used as input; ADC result is right justified
	DIDR0 = (1 << ADC1D);	//Disables digital input buffer circuit of the ADC1 pin to reduce power consumption
	ADCSRA = (1 << ADEN | 1 << ADSC | 7 << ADPS0);	//Enable ADC; Start ADC; Prescaler Value = 128; ADC Clock = 125 KHz
			
    LCD_Init();//clears display and sets display to first char position, first row
	LCD_string("Embedded System");//print string constant to LCD
	LCD_command(0xC0);//go to first char of the 2nd line of the display
	LCD_string("Laboratory");//print string constant to LCD    
	_delay_ms(5000);
	LCD_clear();//clears display
	
    while (1) 
    {
		if (!(ADCSRA & (1<< ADSC)));		//used an if to check ADSC in register ADCSRA; ADSC will be 1    //as long as conversion in progress
		{
			ADC_data = ADC;	//reads the "number" produced by the ADC; reads all 10 bits right justified
			ADC_voltage = (5.0*ADC_data)/(1024);	//calculate the Analog Voltage (float) read by the ADC
			
			//calculate the Temperature (float) from the Analog Voltage and the equation //shown on graph in lab handout
			Temperature = (-3.4169*(pow(ADC_voltage, 3))+26.083*(pow(ADC_voltage, 2))-81.559*ADC_voltage+119.6);
			sprintf(result,"%.2f",Temperature); //copy the calculated temperature into "result" string
			
			LCD_command(0x02);//Sets Display to start at first character of first Line
			LCD_string("Temperature (C):");
			LCD_command(0xC0);//go to first char of the 2nd line of the display
			LCD_string(result);

			ADCSRA |= (1 << ADSC);//start another ADC conversion
			_delay_ms(1500);
		}
    }
	
	return 0;
}
