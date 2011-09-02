#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "main.h"
#include "usart.h"
#include "TWI_Master.h"




void SetLed2(uint8_t x,uint8_t y,uint8_t r,uint8_t g, uint8_t b)
{
/*	uint8_t r2=r;
	uint8_t g2=g;
	uint8_t b2=b;
//	r=r*0.7;
//	g=g*0.7;
//	b=b*0.7;
	
	if((r2>0)&&(r==0)) {r=1;};
	if((g2>0)&&(g==0)) {g=1;};
	if((b2>0)&&(b==0)) {b=1;};

*/
	   	    if (!TWIM_Start (2, TWIM_WRITE))
		    	{
			//		SetLed(1,255,0,0);
	    			TWIM_Stop ();
				}
	        	else
		        {
			//		SetLed(1,0,0,255);
					TWIM_Write (x);
					TWIM_Write (y);
					TWIM_Write (r);
					TWIM_Write (g);
					TWIM_Write (b);
    	    	    TWIM_Stop ();
			//		SetLed(1,0,255,0);
	    	    }
}

uint8_t bytemap[720];

void SetLed3(void)
{
	   	    if (!TWIM_Start (2, TWIM_WRITE))
		    	{
			//		SetLed(1,255,0,0);
	    			TWIM_Stop ();
				}
	        	else
		        {
			//		SetLed(1,0,0,255);
					TWIM_Write (255);
					
					for(uint8_t m = 0;m<49;m++)
					{
						uint16_t midx = (m)*15;
						
						TWIM_Write(bytemap[midx]);
						TWIM_Write(bytemap[midx+1]);
						TWIM_Write(bytemap[midx+2]);
						
						TWIM_Write(bytemap[midx+3]);
						TWIM_Write(bytemap[midx+4]);
						TWIM_Write(bytemap[midx+5]);
						
						TWIM_Write(bytemap[midx+6]);
						TWIM_Write(bytemap[midx+7]);
						TWIM_Write(bytemap[midx+8]);
						
						TWIM_Write(bytemap[midx+9]);
						TWIM_Write(bytemap[midx+10]);
						TWIM_Write(bytemap[midx+11]);
						
						TWIM_Write(bytemap[midx+12]);
						TWIM_Write(bytemap[midx+13]);
						TWIM_Write(bytemap[midx+14]);
					}




					TWIM_Write (255);
    	    	    TWIM_Stop ();
			//		SetLed(1,0,255,0);
	    	    }
}



int main (void)
{


	// enable pull ups for the 4 keys
	PORTA |= (1<<PORTA4)|(1<<PORTA5)|(1<<PORTA6)|(1<<PORTA7);

	// led 4
	DDRC |= (1<<DDC3);
	
	// set LED Pins to output

	// disable bootloader LED
	DDRC |= (1<<DDC3);
	PORTC |= (1<<PORTC3);





	USART_Init ();
	
	sei ();

	

	TWIM_Init (200000);



	_delay_ms(500);

/*	while(1)
	{
	for(uint8_t p = 1;p<50;p++)
	{
		SetLed2(p,1,55,0,105);
		_delay_ms(2);
		SetLed2(p,2,55,0,105);
		_delay_ms(2);
		SetLed2(p,3,55,0,105);
		_delay_ms(2);
		SetLed2(p,4,55,0,105);
		_delay_ms(2);
		SetLed2(p,5,55,0,105);
		_delay_ms(2);
		
	}*/
	for(uint8_t p = 1;p<50;p++)
	{
		SetLed2(p,1,55,105,0);
		_delay_ms(20);
		SetLed2(p,2,55,105,0);
		_delay_ms(20);
		SetLed2(p,3,55,105,0);
		_delay_ms(20);
		SetLed2(p,4,55,105,0);
		_delay_ms(20);
		SetLed2(p,5,55,105,0);
		_delay_ms(20);
		
	}
//	}

	
	uint8_t mode = 0;
	
	uint8_t b0 = 0;
	uint8_t b1 = 0;
	uint8_t b2 = 0;
	uint8_t b3 = 0;
	uint8_t b4 = 0;
	
	uint8_t data = 0;
	uint8_t escape = 0;
	uint16_t idx = 0;
	
	while(1)
	{

		//while (!(UCSR1A & (1 << RXC1)));
		//data = UDR1;
		while (uart_getc_nb(&data))
		{

			
			if(data == 0x42)
			{
				mode = 1;
				continue;
			//	lcd_printp (PSTR("1"), 0);
			}
			if(data == 0x23)
			{
				mode = 6;
			//	lcd_printp (PSTR("2"), 0);
				idx = 0;
				continue;
			}
			if(data == 0x65)
			{
			//	lcd_printp (PSTR("3"), 0);
				escape = 1;
				continue;
			}
			if(data == 0x66)
			{
				asm volatile("jmp 0xE000");
			}
			if(escape == 1)
			{
				escape = 0;
			//	lcd_printp (PSTR("4"), 0);
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
			
		
			if(mode == 1)
			{
				b0 = data;
				mode = 2;
			}
			else if(mode == 2)
			{
				b1 = data;
				mode = 3;
			}
			else if(mode == 3)
			{
				b2 = data;
				mode = 4;
			}
			else if(mode == 4)
			{
				b3 = data;
				mode = 5;
			}
			else if(mode == 5)
			{
				b4 = data;
				mode = 0;
				if(b0 < 50)
				{
					if(b1 < 6)
					{
						SetLed2(b0,b1,b2,b3,b4);
//						USART1_putc(0x42);
					}
				}
			}
			else if(mode == 6)
			{
				bytemap[idx]=data;
				idx++;	
				

				if(idx == 720)
				{
					SetLed3();
					mode = 0;
					idx = 0;
				}
			}
		}
	}


}





