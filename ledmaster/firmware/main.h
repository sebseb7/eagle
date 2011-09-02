/*****************************************************************************
 *   Copyright (C) 2008 Thomas Kaiser, thomas@ft-fanpage.de                  *
 *   Copyright (C) 2009 Peter "woggle" Mack, mac@denich.net                  *
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
 *                                                                           *
 *   Credits to:                                                             *
 *   Holger Buss & Ingo Busker from mikrokopter.de for the MK project + SVN  *
 *                          http://www.mikrokopter.de                        *
 *   Gregor "killagreg" Stobrawa for his version of the MK code              *
 *   Thomas Kaiser "thkais" for the original project. See                    *
 *                          http://www.ft-fanpage.de/mikrokopter/            *
 *                          http://forum.mikrokopter.de/topic-4061-1.html    *
 *   Claas Anders "CaScAdE" Rathje for providing the font and his C-OSD code *
 *                          http://www.mylifesucks.de/oss/c-osd/             *
 *****************************************************************************/

#ifndef _MAIN_H
#define _MAIN_H

//*****************************************************************************
// 
#define set_cs()    (PORTB |=  (1 << PB4))
#define clr_cs()    (PORTB &= ~(1 << PB4))
#define set_reset() (PORTB |=  (1 << PB2))
#define clr_reset() (PORTB &= ~(1 << PB2))
#define set_A0()    (PORTB |=  (1 << PB3))			// Data
#define clr_A0()    (PORTB &= ~(1 << PB3))			// Command
#define set_scl()   (PORTB |=  (1 << PB7))
#define clr_scl()   (PORTB &= ~(1 << PB7))
#define set_si()    (PORTB |=  (1 << PB5))
#define clr_si()    (PORTB &= ~(1 << PB5))



//*****************************************************************************
// 

#endif
