
/*
 * mcc30_ext.c
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

/* Fix for MPLAB C30 v 3.25 compiler
 *
 */

#if defined( __C30__  ) && ( __C30_VERSION__ == 325 )

#include <signal.h> /* for abort() that normally should be 
						in stdlib.h ! But there is a redefinition
						in signal.h ???
					*/
#include <stdio.h>

#ifndef NDEBUG
void
_fassert(int line, const char * file , const char * expr)
{
	printf( "Assertion failed in %s : %d!\n\t%s\n", file, line, expr );
/*	abort();
*/
}
#endif

#endif
