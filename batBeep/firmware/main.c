#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"


uint8_t adc_timeout = 0;
volatile uint8_t do_adc_sleep = 0;

//30hz
ISR (TIMER0_OVF_vect)
{
	adc_timeout++;
	
	// 2Hz == ~ every 0.5 seconds
	if(adc_timeout == 15)
	{
		adc_timeout=0;
		do_adc_sleep=1;
	}
}

ISR(ADC_vect)
{
	current_adc_value = ADC;
	
	
	if( current_adc_value < 610)
	{
		PORTD &= ~(1<<PORTD7);
	}
	else
	{
		PORTD |= (1<<PORTD7);
	}


	if( current_adc_value < 620)
	{
		PORTC |= (1<<PORTC1);
		PORTC |= (1<<PORTC2);
		PORTC |= (1<<PORTC3);
		PORTC &= ~(1<<PORTC4);
	}
	else if(current_adc_value < 662)
	{
		PORTC |= (1<<PORTC1);
		PORTC |= (1<<PORTC2);
		PORTC &= ~(1<<PORTC3);
		PORTC |= (1<<PORTC4);
	}
	else if(current_adc_value < 705)
	{
		PORTC |= (1<<PORTC1);
		PORTC &= ~(1<<PORTC2);
		PORTC &= ~(1<<PORTC3);
		PORTC |= (1<<PORTC4);
	}
	else 
	{
		PORTC &= ~(1<<PORTC1);
		PORTC &= ~(1<<PORTC2);
		PORTC &= ~(1<<PORTC3);
		PORTC |= (1<<PORTC4);
	}

}



int main (void)
{

	PORTC |= (1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4)|(1<<PORTC5);
	PORTB |= (1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2)|(1<<PORTB3)|(1<<PORTB4)|(1<<PORTB5)|(1<<PORTB6)|(1<<PORTB7);
	PORTD |= (1<<PORTD0)|(1<<PORTD1)|(1<<PORTD2)|(1<<PORTD3)|(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);
	
	DDRC  |= (1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4);
	DDRD |= (1<<PORTD7);

	//reduce power
	PRR |= (1<<PRTWI)|(1<<PRTIM1)|(1<<PRTIM2)|(1<<PRSPI)|(1<<PRUSART0);// add one of the timers !


	//set timer0 to normal Mode & prescaler 1024 == 30Hz
	TCCR0B |= (1<<CS02)|(1<<CS00);
	//enable interrupt
	TIMSK0 |= (1<<TOIE0);

	// select adc0 and interval 1.1v reference
	// adc0 pin is connecgted to vcc with a 1/3 divider
	ADMUX |= (1<<REFS1)|(1<<REFS0);
	ADCSRA |= (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1);
	

	SMCR |= (1<<SE);

	sei();

	while(1)
	{
		if(do_adc_sleep == 1)
		{
			do_adc_sleep=0;
			SMCR |= (1<<SM0);
			SMCR &= ~((1<<SM2)|(1<<SM0));
		}
		else
		{
			SMCR &= ~((1<<SM2)|(1<<SM0)|(1<<SM0));
		}

		asm volatile("sleep");
	}
}


