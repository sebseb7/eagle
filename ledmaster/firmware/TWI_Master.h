/*******************************************************
 Author:					Manfred Langemann
 mailto:					Manfred.Langemann ät t-online.de
 Begin of project:			04.01.2008
 Latest version generated:	04.01.2008
 Filename:					TWI_Master.h
 Description:    			TWI Master functions include file

 ********************************************************/
#ifndef _TWIM
#define _TWIM

#define TRUE    1
#define FALSE   0

#define TWIM_READ    1
#define TWIM_WRITE   0

uint8_t	TWIM_Init (uint32_t TWI_Bitrate);
uint8_t TWIM_Start (uint8_t address, uint8_t TWIM_Type);
void	TWIM_Stop (void);
uint8_t	TWIM_Write (uint8_t byte);
uint8_t	TWIM_ReadAck (void);
uint8_t	TWIM_ReadNack (void);

#endif
