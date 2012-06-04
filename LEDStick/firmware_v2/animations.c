#include <inttypes.h>
#include <util/delay.h>

#include "leds.h"
#include "animations.h"
#include "timer.h"
#include "adc.h"
	
void ani_c3d2()
{
	uint8_t loop=1;	
	while(1)
	{	
		SetLed(3,255,200,200);
		_delay_ms(12);
		SetLed(2,255,200,200);
		SetLed(4,255,200,200);
		SetLed(3,0,0,255);
		_delay_ms(12);
		SetLed(1,255,200,200);
		SetLed(5,255,200,200);
		SetLed(2,0,0,255);
		SetLed(4,0,0,255);
		_delay_ms(12);
		SetLed(1,0,0,255);
		SetLed(5,0,0,255);

		SetLed(3,255,200,200);
		_delay_ms(12);
		SetLed(2,255,200,200);
		SetLed(4,255,200,200);
		SetLed(3,0,0,255);
		_delay_ms(12);
		SetLed(1,255,200,200);
		SetLed(5,255,200,200);
		SetLed(2,0,0,255);
		SetLed(4,0,0,255);
		_delay_ms(12);
		SetLed(1,0,0,255);
		SetLed(5,0,0,255);


		SetLed(3,255,200,200);
		_delay_ms(12);
		SetLed(2,255,200,200);
		SetLed(4,255,200,200);
		SetLed(3,0,0,255);
		_delay_ms(12);
		SetLed(1,255,200,200);
		SetLed(5,255,200,200);
		SetLed(2,0,0,255);
		SetLed(4,0,0,255);
		_delay_ms(12);
		SetLed(1,0,0,255);
		SetLed(5,0,0,255);

		_delay_ms(3);
		SetLed(5,255,200,200);
		_delay_ms(3);
		SetLed(4,255,200,200);
		_delay_ms(3);
		SetLed(3,255,200,200);
		SetLed(5,0,0,255);
		_delay_ms(3);
		SetLed(2,255,200,200);
		SetLed(4,0,0,255);
		_delay_ms(3);
		SetLed(1,255,200,200);
		SetLed(3,0,0,255);
		_delay_ms(3);
		SetLed(2,0,0,255);
		_delay_ms(3);
		SetLed(1,0,0,255);
		_delay_ms(3);


		SetLed(1,255,200,200);
		SetLed(5,255,200,200);
		_delay_ms(12);
		SetLed(1,0,0,255);
		SetLed(5,0,0,255);
		SetLed(2,255,200,200);
		SetLed(4,255,200,200);
		_delay_ms(12);
		SetLed(2,0,0,255);
		SetLed(4,0,0,255);
		SetLed(3,255,200,200);
		_delay_ms(12);
		SetLed(3,0,0,255);


		SetLed(1,255,200,200);
		SetLed(5,255,200,200);
		_delay_ms(12);
		SetLed(1,0,0,255);
		SetLed(5,0,0,255);
		SetLed(2,255,200,200);
		SetLed(4,255,200,200);
		_delay_ms(12);
		SetLed(2,0,0,255);
		SetLed(4,0,0,255);
		SetLed(3,255,200,200);
		_delay_ms(12);
		SetLed(3,0,0,255);



		_delay_ms(200);
		loop++;
		if(loop==100)
		{
			break;
		}
	}
}

void ani_rainbow()
{
	uint8_t add = 1;
	int8_t direction = 1;
	uint8_t count = 0;
	while(1)
	{
		for(uint16_t i = 0; i < 0xFFF;i+=add)
		{
			SetLed12(0,i,0xFFF-i,0);
			//_delay_ms(1);
		}
		for(uint16_t i = 0; i < 0xFFF;i+=add)
		{
			SetLed12(0,0xFFF-i,0,i);
			//_delay_ms(1);
		}
		for(uint16_t i = 0; i < 0xFFF;i+=add)
		{
			SetLed12(0,0,i,0xFFF-i);
			//_delay_ms(1);
		}
		if(direction == 1)
		{
			add++;
		}
		else
		{
			add--;
		}
		if(add == 64)
		{
			direction = 0;
		}
		if(add == 1)
		{
			direction = 1;
			count++;
		}
		if(count == 3)
		{
			break;
		}
	}
}	

void ani_battery()
{
// 0-160
		uint16_t adc_tmp = adc_value;
	
		if(adc_tmp < 850)
		{
			adc_tmp = 850;
		}
		if(adc_tmp > 1010)
		{
			adc_tmp = 1010;
		}
		adc_tmp = adc_tmp-850;


	timeout = 5;
	while(1)
	{	
		SetLed(0,0,255,0);
		_delay_ms(adc_tmp);
		SetLed(0,255,0,0);
		_delay_ms((160-adc_tmp));
		if(timeout==0)
		{
			break;
		}
	}
	
	timeout = 3;
	uint8_t i=0;
	while(1)
	{	
		SetLed(0,0,255,0);
		for(i=0;i<adc_tmp;i++) _delay_ms(10);
		SetLed(0,255,0,0);
		for(i=0;i<(160-adc_tmp);i++) _delay_ms(10);
		if(timeout==0)
		{
			break;
		}
	}
}	

void ani_sectors3()
{
	uint16_t delay=8;
		
	while(1)
	{	
		SetLed12(0,0xFFF,0x3FF,0x3FF);
		_delay_ms(delay);
		SetLed12(0,0x3FF,0x3FF,0xFFF);
		_delay_ms(delay);
		SetLed12(0,0x3FF,0xFFF,0x3FF);
		_delay_ms(delay);
		
		if(timeout==0)
		{
			delay*=2;
			timeout=4;
		}
		if(delay==1024)
		{
			break;
		}
	}
}

void ani_sectors12()
{
	uint16_t delay=1;
	uint8_t i = 0;
	
	timeout = 8;
	while(1)
	{	
		
		SetLed12(0,0xFFF,0,0);
		for(i=0;i<delay;i++){_delay_ms(50);}
		SetLed12(0,0xBFF,0x3FF,0);
		for(i=0;i<delay;i++){_delay_ms(50);}
		SetLed12(0,0x7FF,0x7FF,0);
		for(i=0;i<delay;i++){_delay_ms(50);}
		SetLed12(0,0x3FF,0xBFF,0);
		for(i=0;i<delay;i++){_delay_ms(50);}
		SetLed12(0,0,0xFFF,0);
		for(i=0;i<delay;i++){_delay_ms(50);}
		SetLed12(0,0,0xBFF,0x3FF);
		for(i=0;i<delay;i++){_delay_ms(50);}
		SetLed12(0,0,0x7FF,0x7FF);
		for(i=0;i<delay;i++){_delay_ms(50);}
		SetLed12(0,0,0x3FF,0xBFF);
		for(i=0;i<delay;i++){_delay_ms(50);}
		SetLed12(0,0,0,0xFFF);
		for(i=0;i<delay;i++){_delay_ms(50);}
		SetLed12(0,0x3ff,0,0xBFF);
		for(i=0;i<delay;i++){_delay_ms(50);}
		SetLed12(0,0x7ff,0,0x7FF);
		for(i=0;i<delay;i++){_delay_ms(50);}
		SetLed12(0,0xBff,0,0x3FF);
		for(i=0;i<delay;i++){_delay_ms(50);}
		
		if(timeout==0)
		{
			delay*=2;
			timeout=4;
		}
		if(delay==128)
		{
			break;
		}
	}
}
void ani_fullcolor()
{
	timeout=15;
	while(timeout)
	{
		SetLed(0,255,0,0);
		_delay_ms(2000);
		SetLed(0,255,0,255);
		_delay_ms(2000);
		SetLed(0,0,0,255);
		_delay_ms(2000);
		SetLed(0,0,255,255);
		_delay_ms(2000);
		SetLed(0,0,255,0);
		_delay_ms(2000);
		SetLed(0,255,255,0);
		_delay_ms(2000);
	}
}
void ani_redblue()
{
	timeout=15;
	while(timeout)
	{
		SetLed(0,255,0,0);
		_delay_ms(300);
		SetLed(0,0,0,255);
		_delay_ms(300);
	}
}
void ani_greenyellow()
{
	timeout=6;
	while(timeout)
	{
		SetLed12(0,0,0xFFF,0);
		_delay_ms(300);
		SetLed12(0,0x7FF,0x7FF,0);
		_delay_ms(300);
	}
}
void ani_bluewhite()
{
	timeout=6;
	while(timeout)
	{
		SetLed12(0,0,0,0xFFF);
		_delay_ms(400);
		SetLed12(0,0x555,0x555,555);
		_delay_ms(200);
	}
}
