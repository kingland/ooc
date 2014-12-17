
#ifndef BARDAUGHTER_H
#define BARDAUGHTER_H

#include "../libs/ooc.h"

#include "bar.h"

DeclareClass( BarDaughter, Bar );

BarDaughter bardaughter_new( void );
BarDaughter bardaughter_new_with_data( int );
BarDaughter bardaughter_new_with_text( char * text );
BarDaughter bardaughter_new_with_const_text( const char * text );

/* Virtual function definitions
 */

Virtuals( BarDaughter, Bar )

	void	(* bardaughter_virtual)  ( BarDaughter );

EndOfVirtuals;

#endif  /* BARDAUGHTER_H */
