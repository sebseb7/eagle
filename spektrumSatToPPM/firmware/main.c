// bind todo: 
// satellit sendet 15ms nach poweron up flanke
// basis antwortet nach weiteren 60ms mit 4 low-pulsen (8 flankenwechsel) a 126usi anbstand
//
//
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "usart.h"
#include <util/delay.h>

#define PPMCH 8


volatile unsigned int reset_pw = 0;
volatile unsigned char  isr_channel_number = 0;
volatile unsigned int   isr_channel_pw[(PPMCH +1)]; // +1 is for the reset pulse.
volatile uint8_t timeout = 30;
volatile uint16_t timeout2 = 1000;


#define TIMER1_PRESCALER   64
#define TIMER1_PRESCALER_BITS   ((0<<CS02)|(1<<CS01)|(1<<CS00))


#define RC_PPM_FRAME_TIMER_VAL          ((((F_CPU/1000) * 28000)/1000)/TIMER1_PRESCALER) 
#define RC_RESET_PW_TIMER_VAL           ((((F_CPU/1000) * 7500)/1000)/TIMER1_PRESCALER) 
#define RC_PPM_SYNC_PW_VAL              ((((F_CPU/1000) * 300)/1000)/TIMER1_PRESCALER)

void bind(void);

ISR(TIMER1_COMPB_vect)
{
	asm("sei");
	isr_channel_number++;
	if( isr_channel_number >= (PPMCH + 1) ) 
	{
		isr_channel_number = 0; 
		reset_pw = RC_PPM_FRAME_TIMER_VAL; 
	}
	
	if(isr_channel_number < PPMCH)
	{
		OCR1A = isr_channel_pw[isr_channel_number];
		reset_pw -= OCR1A;
	}else{
		OCR1A = reset_pw;
	} 
	
	return;

}

ISR(TIMER1_OVF_vect)
{   
	return;
}
ISR(TIMER0_OVF_vect)
{  

	if(timeout > 0) timeout--;
	if(timeout2 > 0) timeout2--;
	return;
}  



int main(void)
{
	isr_channel_pw[0] = ((((F_CPU/1000) * 1500)/1000)/TIMER1_PRESCALER);
	isr_channel_pw[1] = ((((F_CPU/1000) * 1500)/1000)/TIMER1_PRESCALER);
	isr_channel_pw[2] = ((((F_CPU/1000) * 1500)/1000)/TIMER1_PRESCALER);
	isr_channel_pw[3] = ((((F_CPU/1000) * 1500)/1000)/TIMER1_PRESCALER);
	isr_channel_pw[4] = ((((F_CPU/1000) * 1500)/1000)/TIMER1_PRESCALER);
	isr_channel_pw[5] = ((((F_CPU/1000) * 1500)/1000)/TIMER1_PRESCALER);
	isr_channel_pw[6] = ((((F_CPU/1000) * 1500)/1000)/TIMER1_PRESCALER);
	isr_channel_pw[7] = ((((F_CPU/1000) * 1500)/1000)/TIMER1_PRESCALER);

								
	TCCR1A = (1<<COM1B1) | (0<<COM1B0) | (1<<WGM11) | (1<<WGM10);
	TCCR1B = (1<<WGM13)|(1<<WGM12);
	TCCR1B |= TIMER1_PRESCALER_BITS;
	OCR1A = RC_RESET_PW_TIMER_VAL;
	OCR1B  = RC_PPM_SYNC_PW_VAL;
	TIMSK1 |= (1<<OCIE1B)|(1<<TOIE1);
	TCNT1 = 0; 
	
	

	isr_channel_number = 1;		

	// timer0 with prescaler 8, and interrupt, Int freq: 9433khz , ~0.0001s 
	TCCR0B |= (1<<CS01);
	TIMSK0 |= (1<<TOIE0);


	DDRB |= (1<<PORTB1); //ppm out
	DDRB |= (1<<PORTB2); //ppm out


//	bind();

	USART_Init();
	sei();

	uint8_t data = 0;
	uint8_t old_data = 0;
	uint8_t state = 0;
	int16_t channels[PPMCH];
	int16_t channels_tmp[PPMCH];
	int16_t channels_old[PPMCH];
	uint8_t i = 0;


	for(i=0;i<PPMCH;i++)
	{
		channels[i] = 0;
		channels_old[i] = 0;
	}
	
					
	while(1)
	{
		if(timeout == 0)
		{
			//state = 0;
			timeout = 60; // 6ms
		}
			
		if(uart_getc_nb(&data))
		{
			timeout = 60; //6ms

			if((old_data == 0x03) && (data == 0x01))
			{
				state=0;
			}

			for(i = 2; i < 14;i+=2)
			{
				if(state == i)
				{
					channels[(i/2)-1] = data+((old_data&3)*0xff);
				}

				if(state == 12)
				{
					PORTB ^= (1 << PORTB1);
					for(i=0;i<PPMCH;i++)
					{
						uint16_t pw = (1000 + (channels[i]*1.25)) - 116;
						channels_tmp[i] = ((((F_CPU/1000) * pw)/1000)/TIMER1_PRESCALER);
					}
					cli();
					for(i=0;i<PPMCH;i++)
					{
						isr_channel_pw[i] = channels_tmp[i];
					}
					sei();
					timeout2 = 20000; // 2s

					for(i=0;i<PPMCH;i++)
					{
						channels_old[i] = channels[i];
					}


				}


			}


			if(state < 20)
				state++;
			old_data = data;

		}
	}
}


void bind()
{
	UCSR0B &= ~(1 << RXCIE0); 
    UCSR0B &= ~(1 << RXEN0);  
	PORTD &= ~(1 << PORTD0); _delay_us(116);

	DDRD |= (1<<PORTD2);
	DDRD |= (1<<PORTD3);


	uint8_t i = 0;

	_delay_ms(290);

	PORTD |= (1<<PORTD2);
	PORTD |= (1<<PORTD3);

	while(1) // Wait 10 seconds for spektrum sat connection
	{
		if(PIND & (1 << PORTD0))
	    {
	        break;
	    }
	}
	PORTB ^= (1 << PORTB1);
	
	DDRD |= (1 << DDD0);
	_delay_ms(290);
	PORTD &= ~(1 << PORTD0); _delay_us(116);
	PORTD |= (1 << PORTD0);  _delay_us(116);
	PORTD &= ~(1 << PORTD0); _delay_us(116);
	PORTD |= (1 << PORTD0);  _delay_us(116);
	PORTD &= ~(1 << PORTD0); _delay_us(116);
//	PORTD |= (1 << PORTD0);  _delay_us(116);

	_delay_ms(200);
	PORTB ^= (1 << PORTB1);
	_delay_ms(200);
	PORTB ^= (1 << PORTB1);
	_delay_ms(200);

	DDRD &= ~(1 << DDD0); // Rx as input
	PORTD &= ~(1 << PORTD0);
}

