#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "main.h"
#include "spi.h"
#include "usart.h"


// BLANC == PB2
// XLAT  == PB1
// SCLK == SCK/PB5
// SIN == MOSI/PB3
// SOUT == MISO/PB4


typedef void (*AppPtr_t)(void) __attribute__ ((noreturn)); 

volatile uint8_t pushData = 0;

ISR (TIMER1_OVF_vect)
{
	pushData = 1;
}

int main (void)
{
	// set mosi/sck out
	DDRB = (1<<DDB5)|(1<<DDB3)|(1<<DDB2);
	
	//latch out
	DDRB |= (1<<DDB1);
	//blank out
	DDRD |= (1<<DDD7);

	// latch aus
	PORTB &= ~(1<<PORTB1);
	// blank = high (all LEDs off)
	PORTD |= (1<<PORTD7);
	
	//SPI_Init()
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);

	//fill the RAM of the TLC with defined values
	SetLed(0,0,0,0);
	writeChannels();

	// blank = low (enable LEDs)
	PORTD &= ~(1<<PORTD7);
	
	DDRC &= ~(1<<PORTC0);
	DDRC &= ~(1<<PORTC1);
	DDRC &= ~(1<<PORTC2);
	DDRC &= ~(1<<PORTC3);
	DDRD &= ~(1<<PORTD2);
	DDRD &= ~(1<<PORTD3);
	PORTC |= (1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3);
	PORTD |= (1<<PORTD2)|(1<<PORTD3);

	//enable pullups ununsed pins
	PORTD |= (1<<PORTD4);
	PORTD |= (1<<PORTD5);
	PORTD |= (1<<PORTD6);
	PORTB |= (1<<PORTB0);
	PORTB |= (1<<PORTB2);
	PORTC |= (1<<PORTC4);
	PORTC |= (1<<PORTC5);
	
	//disable input buffers on unused pins
	DIDR1 |= (1<<AIN0D);
	DIDR0 |= (1<<ADC4D);
	DIDR0 |= (1<<ADC5D);
	
	//disable unused hardware (twi,adc,acd,timer0,timer2)
	PRR |= (1<<PRTWI)|(1<<PRADC)|(1<<PRTIM0)|(1<<PRTIM2);
	ACSR |= (1<<ACD); 

	//timer1 for tlc sync

	//set to FastPWM Mode & prescaler 8
	TCCR1A |= (1<<WGM10)|(1<<WGM11);
	TCCR1B |= (1<<WGM12)|(1<<WGM13)|(1<<CS11);//|(1<<CS10);
	//this is one cycle length of the TLC (2560)
	OCR1A = 1112; //2780 looks good
	//enable interrupt
	TIMSK1 |= (1<<TOIE1);
	


	//enable UART RX
	USART0_Init();
	
	//enable interrupts
	sei();

	uint8_t i = 0;
	
	while(1)
	{
		for(i=0;i<10;i++)
		{
			for(uint8_t j=0;j<10-i;j++)
			{
				SetLed(0,255,255,255);
				writeChannels();
				_delay_ms(i*10);
				SetLed(0,0,0,255);
				writeChannels();
				_delay_ms(i*10);
			}
		}

		for(i=0;i<20;i++)
		{
			for(uint8_t j=0;j<10-i;j++)
			{
				SetLed(0,0,255,0);
				writeChannels();
				_delay_ms(i*10);
				SetLed(0,0,0,255);
				writeChannels();
				_delay_ms(i*10);
			}
		}

		for(i=0;i<20;i++)
		{
			for(uint8_t j=0;j<10-i;j++)
			{
				SetLed(0,0,255,0);
				writeChannels();
				_delay_ms(i*10);
				SetLed(0,255,0,0);
				writeChannels();
				_delay_ms(i*10);
			}
		}


		for(uint8_t j=50;j<100;j+=10)
		{
		for(i=0;i<5;i++)
		{
			SetLed(0,255,0,0);
			writeChannels();
			_delay_ms(j);
			SetLed(0,0,0,255);
			writeChannels();
			_delay_ms(j);
		}
		}

		for(uint8_t j=50;j<100;j+=10)
		{
		for(i=0;i<5;i++)
		{
			SetLed(0,0,255,0);
			writeChannels();
			_delay_ms(j);
			SetLed(0,255,255,0);
			writeChannels();
			_delay_ms(j);
		}
		}



		for(uint8_t j=0;j<3;j++)
		{
			for(i=1;i<5;i++)
			{
				SetLed(0,0,0,0);
				SetLed(i*2,255,0,0);
				SetLed(i*2-1,255,0,0);

				SetLed((5-i)*2,0,0,255);
				SetLed((5-i)*2-1,0,0,255);
				writeChannels();
				_delay_ms(200);
			}
			for(i=4;i>1;i--)
			{
				SetLed(0,0,0,0);
				SetLed(i*2,255,0,0);
				SetLed(i*2-1,255,0,0);

				SetLed((5-i)*2,0,0,255);
				SetLed((5-i)*2-1,0,0,255);
				writeChannels();
				_delay_ms(200);
			}
		}



		for(i=0;i<50;i++)
		{
			SetLed(0,255,0,0);
			writeChannels();
			_delay_ms(30);
			SetLed(0,0,255,0);
			writeChannels();
			_delay_ms(30);
			SetLed(0,0,0,255);
			writeChannels();
			_delay_ms(30);
		}

		for(i=0;i<50;i++)
		{
			SetLed(0,255,0,0);
			writeChannels();
			_delay_ms(10);
			SetLed(0,0,255,0);
			writeChannels();
			_delay_ms(10);
			SetLed(0,0,0,255);
			writeChannels();
			_delay_ms(10);
		}

		for(i=0;i<100;i++)
		{
			SetLed(0,0,255,255);
			writeChannels();
			_delay_ms(3);
			SetLed(0,255,0,255);
			writeChannels();
			_delay_ms(3);
			SetLed(0,255,255,0);
			writeChannels();
			_delay_ms(3);
		}


		SetLed(0,0,0,0);

		uint16_t q = 0;
		uint16_t j = 1;
		
		for(q=1024;q>2;)
		{
			for(uint16_t v=0;v<j;v++)
			{
				SetLed(0,0,255,255);
				writeChannels();
				_delay_ms(q);
				SetLed(0,255,0,255);
				writeChannels();
				_delay_ms(q);
				SetLed(0,255,255,0);
				writeChannels();
				_delay_ms(q);
				SetLed(0,255,0,0);
				writeChannels();
				_delay_ms(q);
				SetLed(0,0,255,0);
				writeChannels();
				_delay_ms(q);
				SetLed(0,0,0,255);
				writeChannels();
				_delay_ms(q);
			}
			j = j * 2;
			
			q = q >> 1;
		}

	}


}

