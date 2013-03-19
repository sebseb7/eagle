#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usart.h"


#include "main.h"


uint8_t cycle = 0;

volatile uint8_t led_a = 0;
volatile uint8_t led_b = 0;
volatile uint8_t led_c = 0;
volatile uint8_t led_d = 0;
volatile uint8_t led_a2 = 0;
volatile uint8_t led_b2 = 0;
volatile uint8_t led_c2 = 0;
volatile uint8_t led_d2 = 0;

ISR (TIMER0_OVF_vect)
{
	cycle++;
	
	if(cycle == 0)
	{
		led_a = led_a2;
		led_b = led_b2;
		led_c = led_c2;
		led_d = led_d2;
		
		if(led_a != 0)
		{
			PORTC |= (1<<PORTC2);
		}
		if(led_b != 0)
		{
			PORTC |= (1<<PORTC3);
		}
		if(led_c != 0)
		{
			PORTC |= (1<<PORTC4);
		}
		if(led_d != 0)
		{
			PORTC |= (1<<PORTC5);
		}
	}
	else
	{
		if(led_a == cycle)
		{
			PORTC &= ~(1<<PORTC2);
		}
		if(led_b == cycle)
		{
			PORTC &= ~(1<<PORTC3);
		}
		if(led_c == cycle)
		{
			PORTC &= ~(1<<PORTC4);
		}
		if(led_d == cycle)
		{
			PORTC &= ~(1<<PORTC5);
		}
	}
}

int main(void)
{
	TCCR0B |= (1<<CS00);
	TIMSK0 |= (1<<TOIE0);

	PORTC &= ~((1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4)|(1<<PORTC5));
	DDRC |= (1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4)|(1<<PORTC5);

	DDRD |= (1<<PORTD5);
	PORTD &= ~(1<<PORTD5);


	USART0_Init();
	
	
	sei();
	

	DDRB |= (1<<PORTB0);
	PORTB |= (1<<PORTB0);
	

	led_a2 = 1;//white
	led_b2 = 1;//blue
	led_c2 = 1;//green
	led_d2 = 1;//red
	

	uint8_t idx = 0;
	uint8_t escape = 0;
	uint8_t our_data = 0;

	while(1)
	{
		uint8_t data = 0;

		if(USART0_Getc_nb(&data))
		{

			if(data == 0x42)
			{
				idx = 0;
				escape = 0;
				our_data = 0;
				continue;
				
			} else if (data == 0x23)
			{
				idx=5;
				continue;
			} else if (data == 0x65)
			{
				escape = 1;
				continue;
			}

			if(escape == 1)
			{
				if(data == 1)
				{
					data = 0x23;
				}
				else if (data == 2)
				{
					data = 0x42;
				}
				else if (data == 3)
				{
					data = 0x65;
				}
				else if (data == 4)
				{
					data = 0x66;
				}
				escape = 0;
			}

			if(idx == 0)
			{
				if(data == 0xf3)//addr
				{
					our_data = 1;
				} else if(data == 0xf0)//bcast
				{
					our_data = 1;
				}
				else
				{
					our_data = 0;
				}
			} 
			else if(our_data == 1)
			{
			
				if (idx == 1)
				{
					led_d2 = data;//red
				} else if (idx == 2)
				{
					led_c2  = data;//green
				} else if (idx == 3)
				{
					led_b2 = data;//blue
				} else if (idx == 4)
				{
					led_a2 = data;//white
					
					our_data = 0;
				}
				
			}
			if(idx < 5)
			{
				idx++;
			}
		}

	}

}



