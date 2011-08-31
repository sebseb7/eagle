//
//  main.c
//  SidModule
//


#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "main.h"
#include "usart.h"

void nops(void);
void toggle_cs(void);
void write_reg(uint8_t,uint8_t);


#define SID_RESET_DDR DDRA
#define SID_RESET_PORT PORTA
#define SID_RESET_PIN PORTA4
#define SID_RW_DDR DDRA
#define SID_RW_PORT PORTA
#define SID_RW_PIN PORTA5
#define SID_CS_DDR DDRA
#define SID_CS_PORT PORTA
#define SID_CS_PIN PORTA6

uint8_t sid_buf[25];


int main(void)
{
	SID_RESET_DDR |= (1<<SID_RESET_PIN);
	SID_RESET_PORT |= (1<<SID_RESET_PIN);
	SID_RW_DDR |= (1<<SID_RW_PIN);
	SID_CS_DDR |= (1<<SID_CS_PIN);
	SID_CS_PORT |= (1<<SID_CS_PIN);
	
	DDRC = 0xFF;
//	PORTC = 0xFF;
	
	DDRD = 0x7C;

    USART0_Init();
    sei();
    
    uint8_t data = 0;
    
    while(1)
    {


		if(USART0_Getc_nb(&data))
		{

			// jump to bootloader        
			if(data == 0x1b)
        	{
				GPIOR2=255;
	    	   	asm volatile("jmp 0xE000");
	        }
	        // flush all 25 registers to the SID
	        if(data == 0xf7)
	        {
				for(uint8_t i = 0; i<25;i++)
				{
					write_reg(i,sid_buf[i]);
				}
	        }
	        else
	        {
				uint8_t sid_addr = data;
			
				USART0_Getc(&data);
				
				uint8_t sid_data = data;

				if(sid_addr < 26)
				{
					sid_buf[sid_addr]=sid_data;
					USART0_putc(' ');
				}
				else
				{
					USART0_putc('X');
				}

	        }
	   	}
    }
}

void write_reg(uint8_t sid_addr, uint8_t sid_data)
{

			PORTC = sid_data;
			PORTD &= ~(0x7C);
			PORTD |= (sid_addr<<2);
			
			toggle_cs();
			
			PORTC = 0xff;
			PORTD |= (63<<2);

}

void toggle_cs(void)
{
					SID_CS_PORT &= ~(1<<SID_CS_PIN);
					nops();
					SID_CS_PORT |= (1<<SID_CS_PIN);
					nops();
}

void nops(void)
{
	for(uint8_t i = 0; i<30;i++)
	{
		asm volatile("nop");
	}

}
