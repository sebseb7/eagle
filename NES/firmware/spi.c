#include <avr/io.h>
#include <avr/interrupt.h>

#include "spi.h"

#define SPI_DDR DDRB	// DDR of SPI port
#define SPI_PORT PORTB	// SPI port

#define SPI_MOSI PORTB3	// MOSI pin (Master out, Slave in)
#define SPI_MISO PORTB4	// MISO pin (Master in, Slave out)
#define SPI_SCK PORTB5	// SCK pin (SPI clock)

#define SPI_WAIT()              while ((SPSR & (1<<SPIF)) == 0);

void SPI_Init()
{
    SPI_DDR |= ((1<<SPI_MOSI)|(1<<SPI_SCK));

	SPCR = ((1<<SPE)|(1<<MSTR));
}

void SPI_ReadWrite_Block(uint8_t* data, uint8_t* buffer, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++) {
          SPDR = data[i];
          SPI_WAIT();
          buffer[i] = SPDR;
    }
}

void SPI_Write_Block(uint8_t* data, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++) {
          SPDR = data[i];
          SPI_WAIT();
    }
}

uint8_t SPI_Write_Byte(uint8_t byte)
{
    SPDR = byte;
    SPI_WAIT();
    return SPDR;
}
