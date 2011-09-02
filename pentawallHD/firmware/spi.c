#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>



#include "main.h"
#include "spi.h"

volatile uint8_t newdata = 0;

uint16_t ch[48];

	
uint16_t pwmtable_8[256]  PROGMEM = { 
0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3,
3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5,
6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 10,
10, 10, 11, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16,
16, 17, 17, 18, 19, 19, 20, 21, 22, 22, 23, 24, 25,
26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 37, 38, 39,
41, 42, 44, 45, 47, 49, 50, 52, 54, 56, 58, 60, 62,
64, 67, 69, 72, 74, 77, 79, 82, 85, 88, 91, 95, 98,
102, 105, 109, 113, 117, 121, 126, 130, 135, 140, 145,
150, 155, 161, 166, 172, 179, 185, 192, 198, 206, 213,
221, 228, 237, 245, 254, 263, 272, 282, 292, 303, 314,
325, 337, 349, 361, 374, 387, 401, 416, 431, 446, 462,
479, 496, 513, 532, 551, 571, 591, 612, 634, 657, 680,
705, 730, 756, 783, 811, 840, 870, 902, 934, 967, 1002,
1038, 1075, 1114, 1154, 1195, 1238, 1282, 1328, 1376, 1425,
1476, 1529, 1584, 1640, 1699, 1760, 1823, 1888, 1956, 2026,
2099, 2174, 2252, 2332, 2416, 2502, 2592, 2685, 2781, 2881,
2984, 3091, 3202, 3316, 3435, 3558, 3686, 3818, 3954, 4095 };


//led numbering (1 ist lower left, next to right)
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
		ch[idx[led-1]*3+2]=pgm_read_word(pwmtable_8+blue);
		ch[idx[led-1]*3+1]=pgm_read_word(pwmtable_8+green);
		ch[idx[led-1]*3]=pgm_read_word(pwmtable_8+red);
	}
	else
	{
		for(uint8_t i = 0;i<16;i++)
		{
			ch[idx[i]*3+2]=pgm_read_word(pwmtable_8+blue);
			ch[idx[i]*3+1]=pgm_read_word(pwmtable_8+green);
			ch[idx[i]*3]=pgm_read_word(pwmtable_8+red);
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



									
