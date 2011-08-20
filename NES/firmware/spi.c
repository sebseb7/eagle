#include <avr/io.h>
#include <avr/interrupt.h>

#include "spi.h"

#define SPI_DDR DDRB	// DDR of SPI port
#define SPI_PORT PORTB	// SPI port
#define SPI_MOSI PORTB5	// MOSI pin (Master out, Slave in)
#define SPI_MISO PORTB6	// MISO pin (Master in, Slave out)
#define SPI_SCK PORTB7	// SCK pin (SPI clock)
#define SPI_SS PORTB4	// SS pin (Slave Select)

#define SPI_WAIT()              while ((SPSR & _BV(SPIF)) == 0);

void SPI_Init()
{
    SPI_DDR |= (_BV(SPI_MOSI) | _BV(SPI_SCK) | _BV(SPI_SS));
	SPCR = _BV(SPE) | _BV(MSTR);
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
