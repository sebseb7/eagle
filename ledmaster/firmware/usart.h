/*****************************************************************************
 *   Copyright (C) 2009 Peter "woggle" Mack, mac@denich.net                  *
 *   taken some ideas from the C-OSD code from CaScAdE                       *
 *   the MK communication routines are taken from the MK source              *
 *    (killagreg version)                                                    *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License.          *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.               *
 *                                                                           *
 *****************************************************************************/

#ifndef _USART_H
#define _USART_H

//*****************************************************************************
// 
#ifndef FALSE
#define FALSE	0
#endif
#ifndef TRUE
#define TRUE	1
#endif

#define USART_BAUD 500000	

extern volatile uint8_t xon;

void USART_Init (void);
void USART_putc (char c);
uint8_t uart_getc_nb(uint8_t*);



#endif

