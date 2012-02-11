#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "main.h"
#include "timer.h"
#include "adc.h"
#include "leds.h"
#include "spi.h"
#include "animations.h"


// BLANC == PD7
// XLAT  == PB1
// SCLK == SCK/PB5
// SIN == MOSI/PB3
// MODE == PD5
// XERR == PD2
// SOUT == MISO/PB4
// GSCLK == PB0

volatile uint8_t activation = 0;

ISR(PCINT2_vect)
{
	if(timeout != 0)
	{
		return;
	};
	timeout = 2;
	
	if(activation == 0)
	{
		activation = 1;
	}
	else
	{
		SetLed(0,0,0,0);
		SMCR |= (1<<SM1)|(1<<SE);
		
		// in the end we like to reenable pcie2 also, to have the second button free for mode-change
		
		PCICR &= ~(1<<PCIE2);
		asm volatile("sei");
		asm volatile("sleep");
		wdt_enable(WDTO_250MS);
		while(1);
	}
}

ISR(INT1_vect)
{
	wdt_enable(WDTO_250MS);
}

int main (void)
{
	cli();
	wdt_reset();
	if( (MCUSR & (1<<WDRF))==(1<<WDRF))
	{
		activation = 1;
	}
	MCUSR &= ~(1<<WDRF);
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	WDTCSR = 0x00;
                                    
                                    
	SPI_init();
	LED_init();
	ADC_Init();
	TIMER1_Init();
	
	PORTD |= (1<<PORTD4);
	PCMSK2 |= (1<<PCINT20);
	PCICR |= (1<<PCIE2);
	
	PORTD |= (1<<PORTD3);
//	EICRA |= (1<<ISC11);
	EIMSK |= (1<<INT1);
	
	
	PRR|=(1<<PRTWI)|(1<<PRTIM2)|(1<<PRTIM0)|(1<<PRUSART0);

	sei ();
	
	SetDC(0,63,63,63);
	
	SetLed(0,0,0,0);


	initialized = 1;
	


	while(activation == 0)
	{
		SetLed(1,0x80,0x40,0x40);
		SetLed(2,0x08,0x08,0x08);
		SetLed(3,0,0,10);
		SetLed(4,10,0,0);
		SetLed(5,0,10,0);
	}
	
	
	

	while(1)
	{
		ani_c3d2();	
		ani_battery();	
		ani_rainbow();	
		ani_sectors3();
		ani_bluewhite();	
		ani_greenyellow();	
		ani_redblue();	
		ani_sectors12();
	}// while(1)
	
}	
