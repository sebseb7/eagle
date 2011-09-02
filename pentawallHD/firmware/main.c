// spanungen blau: 3.6
// rot : 2.9
// gruen 3.9

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "main.h"
#include "spi.h"
#include "usart.h"


//compute our position from this using addr
#define DISPLAY_WIDTH 24
#define DISPLAY_HEIGHT 24
                                                                                                                                 // BLANC == PB2
// XLAT  == PB1
// SCLK == SCK/PB5
// SIN == MOSI/PB3
// SOUT == MISO/PB4


typedef void (*AppPtr_t)(void) __attribute__ ((noreturn)); 

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

	// blank = low (enable LEDs)
	PORTD &= ~(1<<PORTD7);


	// get addr from solder jumpers
	PORTC |= (1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3);
	PORTD |= (1<<PORTD2)|(1<<PORTD3);
	uint8_t addr = (PINC | 0x0F)&((PIND | 0x0C)<<2);


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
	
	//disable unused hardware (twi,adc,acd,timer0,timer1,timer2)
	PRR |= (1<<PRTWI)|(1<<PRADC)|(1<<PRTIM0)|(1<<PRTIM1)|(1<<PRTIM2);
	ACSR |= (1<<ACD); 

	//enable UART RX
	USART0_Init();
	
	//enable interrupts
	sei();


	//dummy initialization code (needs to be replaced)
	for(uint8_t i = 1;i<17;i++)
	{
		SetLed(i,90,0,0); 
		_delay_ms(10);
	}
	for(uint8_t i = 1;i<17;i++)
	{
		SetLed(i,0,90,0); 
		_delay_ms(10);
	}
	for(uint8_t i = 1;i<17;i++)
	{
		SetLed(i,0,0,90); 
		_delay_ms(10);
	}
	for(uint8_t i = 1;i<17;i++)
	{
		SetLed(i,0,0,0);
		_delay_ms(10);
	}

	
	
	uint8_t data = 0;  
	uint8_t state = 0;
	uint8_t escape = 0;
	uint16_t idx = 0;

	while(1)
	{
		if(USART0_Getc_nb(&data))
		{

			if(data == 0x42)
			{
				// single pixel
				state = 1;
				idx = 0;
				continue;
			}
			if(data == 0x23)
			{
				// full frame
				state = 2;
				idx = 0;
				continue;
			}
			if(data == 0x65)
			{
				escape = 1;
				continue;
			}
			if(data == 0x66)
			{
				// jump to bootloader
				GPIOR2=255;
				AppPtr_t AppStartPtr = (AppPtr_t)0x1800; 
				AppStartPtr();
			}
			if(escape == 1)
			{
				escape = 0;
				if(data == 0x01)
				{
					data = 0x23;
				}
				else if(data == 0x02)
				{
					data = 0x42;
				}
				else if(data == 0x03)
				{
					data = 0x65;
				}
				else if(data == 0x04)
				{
					data = 0x66;
				}
			}
			
		
			if(state == 1)
			{
				// wait for our pixel
				idx++;
			}

			if(state == 2)
			{
				// wait for our part of the frame
				idx++;
			}
		}
	}
}
