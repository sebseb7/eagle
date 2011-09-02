#include <avr/io.h>

#include "spi.h"

volatile uint8_t newdata = 0;

uint16_t ch[48];



uint8_t idx[16] = {10,11,12,13,9,8,15,14,6,7,0,1,5,4,3,2};

void SPI_send(uint8_t cData) 
{
	SPDR = cData; 
	while(!(SPSR & (1<<SPIF)));
}


void SetLed(uint8_t led,uint8_t red,uint8_t green, uint8_t blue)
{

	if(led > 0)
	{
		ch[idx[led-1]*3+2]=blue<<4;
		ch[idx[led-1]*3+1]=green<<4;
		ch[idx[led-1]*3]=red<<4;
	}
	else
	{
		for(uint8_t i = 0;i<16;i++)
		{
			ch[idx[i]*3+2]=blue<<4;
			ch[idx[i]*3+1]=green<<4;
			ch[idx[i]*3]=red<<4;
		}
	
	}


	for(uint8_t i = 24;i>0;i--)
	{
		SPI_send(ch[i*2-1]>>4);
		SPI_send((ch[i*2-1]<<4)|(ch[i*2-2]>>8));
		SPI_send(ch[i*2-2]);
	}


//	PORTD |= (1<<PORTD7);//blanc on
	PORTB |= (1<<PORTB1); // latch on
//	_delay_ms(1);
	PORTB &= ~(1<<PORTB1); // latch off
//	PORTD &= ~(1<<PORTD7);//blanc off

}

