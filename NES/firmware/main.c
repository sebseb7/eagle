#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include <util/atomic.h>
//#include <avr/wdt.h>


#include "main.h"
#include "radio.h"
#include "led.h"
#include "keys.h"


volatile uint8_t keyState = 0 ; // bit0..7 : up,down,left,right,select,start,a,b

volatile uint8_t blink = 0;
volatile uint8_t timeout = 0;

uint8_t my_addr[5] = { 0x98, 0x76, 0x54, 0x32, 0x11 };
uint8_t station_addr[5] = { 0xE4, 0xE4, 0xE4, 0xE4, 0xE4 };

volatile uint8_t keyInt = 0;
uint8_t sendKeys = 0;

void checkKeys(void);

void checkOff(void);
void powerOff(void);

#define POWEROFF_PORT PORTC 
#define POWEROFF_PINN PORTC5
#define POWEROFF_DDR DDRC

#define USBSENSE_PORT PORTD
#define USBSENSE_PINN PORTD2


ISR(PCINT0_vect)
{
	keyInt = 1;
	inthandl();
}

ISR(PCINT1_vect)
{
	keyInt = 1;
}

ISR(PCINT2_vect)
{
	keyInt = 1;
//	inthandl();
}

ISR(PCINT3_vect)
{
	keyInt = 1;
}


ISR(TIMER1_OVF_vect)
{
	blink++;
	if(blink == 20)
	{
		blink = 0;
		LED4_TOGGLE;
		timeout++;
		sendKeys = 1;
	}
	if(timeout == 200)
	{
		powerOff();
	}
	
//	wdt_reset();
	
}




int main (void)
{
	// 4s watchdog timer
	WDTCSR |= (1<<WDCE); 
	WDTCSR |= (1<<WDP3); 
	
	
	//pull-ups for all switches
	
	KEY_UP_PORT     |= (1<<KEY_UP_PINN);
	KEY_DOWN_PORT   |= (1<<KEY_DOWN_PINN);
	KEY_LEFT_PORT   |= (1<<KEY_LEFT_PINN);
	KEY_RIGHT_PORT  |= (1<<KEY_RIGHT_PINN);

	KEY_A_PORT      |= (1<<KEY_A_PINN);
	KEY_B_PORT      |= (1<<KEY_B_PINN);

	KEY_START_PORT  |= (1<<KEY_START_PINN);
	KEY_SELECT_PORT |= (1<<KEY_SELECT_PINN);


	PORTA |= (1<<PORTA4);//IRQ		PCINT4	/ PCIE0
	PCMSK0 |= (1<<PCINT4);//IRQ


	// pullup for usbsense
//	USBSENSE_PORT |= (1<<USBSENSE_PINN);


	//enable LED PINs as output
	LED1_DDR |= (1<<LED1_PINN);
	LED2_DDR |= (1<<LED2_PINN);
	LED3_DDR |= (1<<LED3_PINN);
	LED4_DDR |= (1<<LED4_PINN);

	if( (MCUSR & (1<<WDRF)) == (1<<WDRF))
	{
		LED1_ON;
	}
	else
	{
		LED1_OFF;
	}
	if( (MCUSR & (1<<BORF)) == (1<<BORF))
	{
		LED2_ON;
	}
	else
	{
		LED2_OFF;
	}
	if( (MCUSR & (1<<EXTRF)) == (1<<EXTRF))
	{
		LED3_ON;
	}
	else
	{
		LED3_OFF;
	}
	if( (MCUSR & (1<<PORF)) == (1<<PORF))
	{
		LED4_ON;
	}
	else
	{
		LED4_OFF;
	}
	_delay_ms(600);


	// set OFF as output
	POWEROFF_DDR |= (1<<POWEROFF_PINN);


	//enable PCINT
	KEY_UP_PCMSK     |= (1<<KEY_UP_PCINT);
	KEY_DOWN_PCMSK   |= (1<<KEY_DOWN_PCINT);
	KEY_LEFT_PCMSK   |= (1<<KEY_LEFT_PCINT);
	KEY_RIGHT_PCMSK  |= (1<<KEY_RIGHT_PCINT);
	KEY_A_PCMSK      |= (1<<KEY_A_PCINT);
	KEY_B_PCMSK      |= (1<<KEY_B_PCINT);
	KEY_SELECT_PCMSK |= (1<<KEY_SELECT_PCINT);
	KEY_START_PCMSK  |= (1<<KEY_START_PCINT);



//	PCICR |= (1<<PCIE0)|(1<<PCIE1)|(1<<PCIE2);
	PCICR |= (1<<PCIE0)|(1<<PCIE1)|(1<<PCIE2)|(1<<PCIE3);
	 

	//timer1
	TCCR1A |= (1<<WGM10)|(1<<WGM11);
	TCCR1B |= (1<<WGM12)|(1<<WGM13)|(1<<CS11)|(1<<CS10);
	OCR1A = 0x3FFF;
	TIMSK1 |= (1<<TOIE1);


	sei();


    Radio_Init();
	_delay_ms(50);
	Radio_Configure_Rx(RADIO_PIPE_0, my_addr, ENABLE);
	Radio_Configure(RADIO_1MBPS, RADIO_HIGHEST_POWER);
                


	while(1)
	{
		if(keyInt == 1)
		{
			keyInt = 0;
			//ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			//{
				checkKeys();
			//}
		}
		if(sendKeys == 1)
		{
			sendKeys = 0;
			LED2_TOGGLE;
//			uint8_t bat = 0;
//			RadioSend(keyState,bat);


			radiopacket_t packet;
			packet.type = MESSAGE;
			memcpy(packet.payload.message.address, my_addr, RADIO_ADDRESS_LENGTH);
			packet.payload.message.messageid = 55;

            packet.payload.message.messagecontent[0] = 1;
			packet.payload.message.messagecontent[1] = keyState;
			packet.payload.message.messagecontent[2] = 100;
                                    
		 	Radio_Set_Tx_Addr(station_addr);
	 
		 	Radio_Transmit(&packet, RADIO_RETURN_ON_TX);
		// 	Radio_Transmit(&packet, RADIO_WAIT_FOR_TX);
		}
	}
}


void checkKeys(void)
{
	uint8_t tmpKeyState=keyState;
	
	for(uint8_t i=0;i<30;i++)
	{
		uint8_t tmpKeyState2=0;
		if((KEY_START_PIN & (1<<KEY_START_PINN)) != (1<<KEY_START_PINN) )
		{
			tmpKeyState2 |= (1<<KEY_START);
		}
		if((KEY_RIGHT_PIN & (1<<KEY_RIGHT_PINN)) != (1<<KEY_RIGHT_PINN) )
		{
			tmpKeyState2 |= (1<<KEY_RIGHT);
		}
		if((KEY_LEFT_PIN & (1<<KEY_LEFT_PINN)) != (1<<KEY_LEFT_PINN) )
		{
			tmpKeyState2 |= (1<<KEY_LEFT);
		}
		if((KEY_UP_PIN & (1<<KEY_UP_PINN)) != (1<<KEY_UP_PINN) )
		{
			tmpKeyState2 |= (1<<KEY_UP);
		}
		if((KEY_DOWN_PIN & (1<<KEY_DOWN_PINN)) != (1<<KEY_DOWN_PINN) )
		{
			tmpKeyState2 |= (1<<KEY_DOWN);
		}
		if((KEY_SELECT_PIN & (1<<KEY_SELECT_PINN)) != (1<<KEY_SELECT_PINN) )
		{
			tmpKeyState2 |= (1<<KEY_SELECT);
		}
		if((KEY_A_PIN & (1<<KEY_A_PINN)) != (1<<KEY_A_PINN) )
		{
			tmpKeyState2 |= (1<<KEY_A);
		}
		if((KEY_B_PIN & (1<<KEY_B_PINN)) != (1<<KEY_B_PINN) )
		{
			tmpKeyState2 |= (1<<KEY_B);
		}
		if(tmpKeyState != tmpKeyState2)
		{
			i=0;
			tmpKeyState=tmpKeyState2;
		}
		_delay_us(100);
	}
	
	//	LED2_TOGGLE;

	if(keyState != tmpKeyState)
	{
		sendKeys = 1;
		timeout = 0;
		keyState = tmpKeyState;	
		LED1_TOGGLE;
	}
	
	checkOff();	
}


void checkOff(void)
{
	if(keyState == ((1<<KEY_A)|(1<<KEY_B)|(1<<KEY_SELECT)) )
//	if(keyState == (1<<KEY_SELECT))
	{
		powerOff();
	}
}


void powerOff(void)
{
	//diable all interrupts
	cli();
	
	
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	_delay_ms(140);
		
	for(uint8_t i = 0;i<5;i++)
	{
		LED1_TOGGLE;
		LED2_TOGGLE;
		LED3_TOGGLE;
		LED4_TOGGLE;
		_delay_ms(140);
	}
	while(1)
	{
		POWEROFF_PORT ^= (1<<POWEROFF_PINN);
		_delay_ms(20);
		LED4_TOGGLE;
	}
}

		


void radio_rxhandler(uint8_t pipe_number)
{

	radiopacket_t packet;

	if (Radio_Receive(&packet) != RADIO_RX_MORE_PACKETS)
	{
		// if there are no more packets on the radio, clear the receive flag;
		// otherwise, we want to handle the next packet on the next loop iteration.
	}

	
	if (packet.type == ACK)
	{
		LED3_TOGGLE;
	}
}

