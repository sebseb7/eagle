#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "main.h"
#include "timer.h"
#include "adc.h"
#include "spi.h"

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

	uint16_t ch1 = 0x0; // 
	uint16_t l1g = 0x0; // links 1 gruen
	uint16_t l1r = 0x0; // links 1 rot
	uint16_t l1b = 0x0; // links 1 blau
	uint16_t l2g = 0x0;
	uint16_t l2r = 0x0;
	uint16_t l2b = 0x0;
	uint16_t l3g = 0x0;
	uint16_t l3r = 0x0;
	uint16_t l5g = 0x0;
	uint16_t l4g = 0x0;
	uint16_t l4b = 0x0;
	uint16_t l4r = 0x0;
	uint16_t l3b = 0x0;
	uint16_t l5r = 0x0;
	uint16_t l5b = 0x0;


	uint8_t dcch1 = 0x0; 
	uint8_t dc1g = 0x0; // links 1 gruen
	uint8_t dc1r = 0x0; // links 1 rot
	uint8_t dc1b = 0x0; // links 1 blau
	uint8_t dc2g = 0x0;
	uint8_t dc2r = 0x0;
	uint8_t dc2b = 0x0;
	uint8_t dc3g = 0x0;
	uint8_t dc3r = 0x0;
	uint8_t dc5g = 0x0;
	uint8_t dc4g = 0x0;
	uint8_t dc4b = 0x0;
	uint8_t dc4r = 0x0;
	uint8_t dc3b = 0x0;
	uint8_t dc5r = 0x0;
	uint8_t dc5b = 0x0;


void SetLed(uint8_t led,uint8_t red,uint8_t green, uint8_t blue)
{
	if(adc_value < 850)
	{
		red   = red>>1;
		green = green>>1;
		blue =  blue>>1;
	} 
    if(adc_value < 800)
	{
		red   = red>>2;
		green = green>>2;
		blue =  blue>>2;
	} 


	while(newdata!=0){};
	PORTD &= ~(1<<PORTD5); // mode = GS
	if((led==1)||(led==0))
	{
		l1r=pgm_read_word(pwmtable_8+red);
		l1g=pgm_read_word(pwmtable_8+green);
		l1b=pgm_read_word(pwmtable_8+blue);
	}
	if((led==2)||(led==0))
	{
		l2r=pgm_read_word(pwmtable_8+red);
		l2g=pgm_read_word(pwmtable_8+green);
		l2b=pgm_read_word(pwmtable_8+blue);
	}
	if((led==3)||(led==0))
	{
		l3r=pgm_read_word(pwmtable_8+red);
		l3g=pgm_read_word(pwmtable_8+green);
		l3b=pgm_read_word(pwmtable_8+blue);
	}
	if((led==4)||(led==0))
	{
		l4r=pgm_read_word(pwmtable_8+red);
		l4g=pgm_read_word(pwmtable_8+green);
		l4b=pgm_read_word(pwmtable_8+blue);
	}
	if((led==5)||(led==0))
	{
		l5r=pgm_read_word(pwmtable_8+red);
		l5g=pgm_read_word(pwmtable_8+green);
		l5b=pgm_read_word(pwmtable_8+blue);
	}
	// 24 byte GS

		SPI_send(ch1>>4);
		SPI_send((ch1<<4)|(l1r>>8));
		SPI_send(l1r);
		SPI_send(l1g>>4);
		SPI_send((l1g<<4)|(l1b>>8));
		SPI_send(l1b);
		SPI_send(l2r>>4);
		SPI_send((l2r<<4)|(l2g>>8));
		SPI_send(l2g);
		SPI_send(l2b>>4);
		SPI_send((l2b<<4)|(l3b>>8));
		SPI_send(l3b);
		SPI_send(l3g>>4);
		SPI_send((l3g<<4)|(l3r>>8));
		SPI_send(l3r);
		SPI_send(l4b>>4);
		SPI_send((l4b<<4)|(l4g>>8));
		SPI_send(l4g);
		SPI_send(l4r>>4);
		SPI_send((l4r<<4)|(l5r>>8));
		SPI_send(l5r);
		SPI_send(l5g>>4);
		SPI_send((l5g<<4)|(l5b>>8));
		SPI_send(l5b);

	newdata=1;
}

void SetLed12(uint8_t led,uint16_t red,uint16_t green, uint16_t blue)
{
//	red = red * 0.7;
//	blue = blue * 0.7;
//	green = green * 0.7;

	if(adc_value < 850)
	{
		red   = red>>3;
		green = green>>3;
		blue =  blue>>3;
	} 
	if(adc_value < 800)
	{
		red   = red>>6;
		green = green>>6;
		blue =  blue>>6;
	} 


	while(newdata!=0){};
	PORTD &= ~(1<<PORTD5); // mode = GS
	if((led==1)||(led==0))
	{
		l1r=red;
		l1g=green;
		l1b=blue;
	}
	if((led==2)||(led==0))
	{
		l2r=red;
		l2g=green;
		l2b=blue;
	}
	if((led==3)||(led==0))
	{
		l3r=red;
		l3g=green;
		l3b=blue;
	}
	if((led==4)||(led==0))
	{
		l4r=red;
		l4g=green;
		l4b=blue;
	}
	if((led==5)||(led==0))
	{
		l5r=red;
		l5g=green;
		l5b=blue;
	}
	// 24 byte GS

		SPI_send(ch1>>4);
		SPI_send((ch1<<4)|(l1r>>8));
		SPI_send(l1r);
		SPI_send(l1g>>4);
		SPI_send((l1g<<4)|(l1b>>8));
		SPI_send(l1b);
		SPI_send(l2r>>4);
		SPI_send((l2r<<4)|(l2g>>8));
		SPI_send(l2g);
		SPI_send(l2b>>4);
		SPI_send((l2b<<4)|(l3b>>8));
		SPI_send(l3b);
		SPI_send(l3g>>4);
		SPI_send((l3g<<4)|(l3r>>8));
		SPI_send(l3r);
		SPI_send(l4b>>4);
		SPI_send((l4b<<4)|(l4g>>8));
		SPI_send(l4g);
		SPI_send(l4r>>4);
		SPI_send((l4r<<4)|(l5r>>8));
		SPI_send(l5r);
		SPI_send(l5g>>4);
		SPI_send((l5g<<4)|(l5b>>8));
		SPI_send(l5b);

	newdata=1;
}

void SetDC(uint8_t led,uint8_t red,uint8_t green, uint8_t blue)
{
//	red = red * 0.7;
//	blue = blue * 0.7;
//	green = green * 0.7;


	while(newdata!=0){};
	PORTD |= (1<<PORTD5); // mode = DC
	PORTD &= ~(1<<PORTD5); // mode = DC
	PORTD |= (1<<PORTD5); // mode = DC

	if((led==1)||(led==0))
	{
		dc1r=red;
		dc1g=green;
		dc1b=blue;
	}
	if((led==2)||(led==0))
	{
		dc2r=red;
		dc2g=green;
		dc2b=blue;
	}
	if((led==3)||(led==0))
	{
		dc3r=red;
		dc3g=green;
		dc3b=blue;
	}
	if((led==4)||(led==0))
	{
		dc4r=red;
		dc4g=green;
		dc4b=blue;
	}
	if((led==5)||(led==0))
	{
		dc5r=red;
		dc5g=green;
		dc5b=blue;
	}

	// 12 byte DC

		SPI_send((dcch1<<2)|(dc1g>>4));
		SPI_send((dc1g<<4)|(dc1r>>2));
		SPI_send((dc1r<<6)|(dc1b>>0));

		SPI_send((dc2g<<2)|(dc2r>>4));
		SPI_send((dc2r<<4)|(dc2b>>2));
		SPI_send((dc2b<<6)|(dc3g>>0));

		SPI_send((dc3r<<2)|(dc5g>>4));
		SPI_send((dc5g<<4)|(dc4g>>2));
		SPI_send((dc4g<<6)|(dc4b>>0));

		SPI_send((dc4r<<2)|(dc3b>>4));
		SPI_send((dc3b<<4)|(dc5r>>2));
		SPI_send((dc5r<<6)|(dc5b>>0));

	/*	SPI_send(0xff);
		SPI_send(0xff);
		SPI_send(0xff);

		SPI_send(0xff);
		SPI_send(0xff);
		SPI_send(0xff);

		SPI_send(0xff);
		SPI_send(0xff);
		SPI_send(0xff);

		SPI_send(0xff);
		SPI_send(0xff);
		SPI_send(0xff);*/



	newdata=1;
}



									
