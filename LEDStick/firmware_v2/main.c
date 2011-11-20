
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

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


int main (void)
{

	

	// latch aus
	PORTB &= ~(1<<PORTB1);
	// blank = high (all off)
	PORTD |= (1<<PORTD7);
	// mode = ground
	PORTD &= ~(1<<PORTD5);
//	PORTD |= (1<<PORTD5);

	//latch out
	DDRB |= (1<<DDB1);
	//blank out
	DDRD |= (1<<DDD7);
	//mode out
	DDRD |= (1<<DDD5);


	SPI_init();
	TIMER1_Init();
	ADC_Init();

	sei ();
	
	SetDC(0,63,63,63);
	
	SetLed(0,0,0,0);

	initialized = 1;

	SetLed(0,0,10,0);
	timeout = 2;
	while(timeout);


	while(1)
	{
		ani_battery();	
		ani_rainbow();	
		ani_c3d2();	
		ani_bluewhite();	
		ani_greenyellow();	
		ani_redblue();	
		ani_sectors();
	}// while(1)
	
}	
