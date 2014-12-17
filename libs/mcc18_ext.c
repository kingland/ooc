
/*
 * mcc18_ext.c
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

#ifdef __18CXX

#include "ooc.h"

#include <stdio.h>


#ifndef NDEBUG

static ROM_ALLOC
char ooc_assert_msg[] = "*** Assertion failed in %HS:%d ***\n";

void
_fassert( int line, ROM char * file )
{
	printf( ooc_assert_msg, file, line );
	abort();
}

#endif

#endif /* __18CXX */

