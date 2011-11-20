#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "main.h"
#include "timer.h"
#include "adc.h"

volatile uint8_t timeout = 0;
volatile uint8_t newdata = 0;
volatile uint8_t initialized = 0;

uint16_t adc_event = 0;


ISR(TIMER1_COMPA_vect)
{
	PORTD |= (1<<PORTD7);//blanc on

	if(newdata == 1)
	{
		PORTB |= (1<<PORTB1); // latch on
		PORTB &= ~(1<<PORTB1); // latch off
	                
		newdata=0;
	}

	if(initialized==1) PORTD &= ~(1<<PORTD7);//blanc off
	
	
	adc_event++;
	if(adc_event == 0xFFF)
	{	
		adc_event=0;
		ADCSRA |= (1<<ADSC);

		if(timeout != 0)
		{
			timeout--;
		}
	}

}



//*****************************************************************************
// 
void TIMER1_Init (void)
{
	
	//WGM12 == CTC mode 
	//CS10 == no prescaling
	// timer runs at 4,9kHz
	
	TCCR1B |= (1<<WGM12)|(1<<CS10);
	OCR1A = 0xFFF;
	TIMSK1 |= (1<<OCIE1A);

	
}


