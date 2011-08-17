#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>


#include "radio.h"


void RadioInit(void)
{

	//radio off during initialization
    CE_LOW;

	//CE & CSN as output    
	CE_DDR |= (1<<CE_PINN);
    CSN_DDR |= (1<<CSN_PINN);
    
    //wait for the radio come up
    _delay_ms(20);
    
    
    //minimum pause between power-states
    _delay_ms(2);
	// power radio (enabled as receiver)
	CE_HIGH;

}


void RadioSend(uint8_t keyState,uint8_t bat)
{
}

