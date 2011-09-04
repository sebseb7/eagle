#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"

uint8_t volatile state = 0;
uint8_t volatile bstate = 0;
uint8_t volatile cstate = 0;

#define TIMER_FREQ = 800

uint8_t volatile leds[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void setLed(uint8_t);



ISR (TIMER1_OVF_vect)
{
	if(state == 0)
	{
		PORTB = 0;
		PORTC = 0;
		PORTD = 0;
	}
	if(state == cstate)
	{
		PORTB = 0xff;
	}
	if(state == 20)
	{
		PORTC = 0xff;
	}
	if(state == 50)
	{
		PORTD = 0xff;
	}
	state++;
	if(state == 250)
	{
		state = 0;
		bstate++;
	}
	if(bstate == 3)
	{
		bstate=0;
		cstate++;
		if(cstate==50)
		{
			cstate = 0;
		}
	}

}
ISR (PCINT2_vect)
{
}

int main (void)
{

	//enable LED channels as output
	PORTB |= (1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2)|(1<<PORTB3)|(1<<PORTB4)|(1<<PORTB5)|(1<<PORTB6)|(1<<PORTB7);
	PORTC |= (1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4);
	PORTD |= (1<<PORTD2)|(1<<PORTD3)|(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);
	
	DDRB |= (1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2)|(1<<PORTB3)|(1<<PORTB4)|(1<<PORTB5)|(1<<PORTB6)|(1<<PORTB7);
	DDRC |= (1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4);//C6 is reset, C5 is ADC, c7 is not available
	DDRD |= (1<<PORTD2)|(1<<PORTD3)|(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);// d0 is RX, d1 is TX


	//reduce power
	PRR |= (1<<PRTWI)|(1<<PRTIM2)|(1<<PRTIM0)|(1<<PRSPI)|(1<<PRUSART0)|(1<<PRADC);

	//disale input pin C5 (adc pin)
	DIDR0 |= (1<<ADC5D);
	
	
	//enable pullups for buttons
	PORTD |= (1<<PORTD0)|(1<<PORTD1);
	
	//enable pcint
	PCMSK2 |= (1<<PCINT16)|(1<<PCINT17);
	PCICR = (1<<PCIE2);
	

	//set to FastPWM Mode & prescaler 1
	TCCR1A |= (1<<WGM11)|(1<<WGM10);
	TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS10);
	OCR1A = 200;//TIMER_FREQ; 
	//enable interrupt
	TIMSK1 |= (1<<TOIE1);

	SMCR = (1<<SE);


	sei();


	while(1)
	{
//		asm volatile("sleep");
	}
}


void setLed(led_nr uint8_t, val uint8_t)
{
	if(led_nr < 20)
	{
		leds[led_nr] = val;
	}
}
