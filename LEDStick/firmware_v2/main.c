
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

	LED_init();
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
