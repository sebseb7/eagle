#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdarg.h>

#include "main.h"
#include "usart.h"



#define UART_RXBUFSIZE 1500

volatile static uint8_t rxbuf[UART_RXBUFSIZE];
volatile static uint8_t *volatile rxhead, *volatile rxtail;
volatile uint8_t xon = 0;

//*****************************************************************************
// 
void USART_Init (void)
{
	// set clock divider
	#undef BAUD
	#define BAUD USART_BAUD
	#include <util/setbaud.h>
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
#if USE_2X
	UCSR0A |= (1 << U2X0);	// enable double speed operation
#else
	UCSR0A &= ~(1 << U2X0);	// disable double speed operation
#endif
	
	// set 8N1
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	UCSR0B &= ~(1 << UCSZ02);

	// flush receive buffer
	while ( UCSR0A & (1 << RXC0) ) UDR0;

	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	UCSR0B |= (1 << RXCIE0);

	rxhead = rxtail = rxbuf;

}



//*****************************************************************************
// 
void USART_putc (char c)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
}



uint8_t uart_getc_nb(uint8_t *c)
{
	if (rxhead==rxtail) return 0;
	*c = *rxtail;
	if (++rxtail == (rxbuf + UART_RXBUFSIZE)) rxtail = rxbuf;



	uint8_t	diff = rxhead - rxtail;
			if((diff < 10)&&(xon==1))
			{
			    PORTC |= (1<<PORTC3);
			    xon=0;
			    USART_putc(0x11);
			}

	return 1;
}


ISR (USART0_RX_vect)
{

                                                                                                        	
		int diff;
		uint8_t c;
		c=UDR0;
		diff = rxhead - rxtail;
		if (diff < 0) diff += UART_RXBUFSIZE;
		if (diff < UART_RXBUFSIZE -1) 
		{
			*rxhead = c;
			++rxhead;
			if (rxhead == (rxbuf + UART_RXBUFSIZE)) rxhead = rxbuf;
			if((diff > 500)&&(xon==0))
			{
			    PORTC &= ~(1<<PORTC3);
			    xon=1;
			    USART_putc(0x13);
			}
		}
		else
		{
		};
		
}


