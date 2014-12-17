/*
 * mplabSIMc32Ext.c
 * 
 * Copyright (c) 2011, old_fashioned. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */

#ifdef __PIC32MX__

/* This file includes an extension for the MPLAB C32 compiler, to be able
 * to use the UART1 in the MPLAB SIM simulator.
 * The _mon_putc() method must be overriden to use the UART1 instead of UART2
 * for the printf functionality.
 * See the C32 Library Guide for the _mon_putc() documentation.
 *
 * Since this file includes <p32xxxx.h> that uses unnamed structs,
 * you can not compile this file with the -ansi compiler option.
 */

#include <p32xxxx.h>

void
_mon_putc( unsigned char c )
{
	static int initialized = 0;

	if( !initialized )
	{
		U1MODEbits.UARTEN = 1;
		U1STAbits.UTXEN = 1;
		initialized = 1;
	}

	while (U1STAbits.UTXBF);
	U1TXREG = c;
}
 
#endif /* __PIC32MX__ */
