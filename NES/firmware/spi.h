#ifndef SPI_H_
#define SPI_H_

#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>

void SPI_Init(void);
void SPI_ReadWrite_Block (uint8_t * data, uint8_t * buffer, uint8_t len);
void SPI_Write_Block (uint8_t * data, uint8_t len);
uint8_t SPI_Write_Byte (uint8_t data);



#endif /* SPI_H_ */
