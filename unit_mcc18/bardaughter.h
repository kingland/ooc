
#ifndef BARDAUGHTER_H
#define BARDAUGHTER_H

#include "../libs/ooc.h"

#include "bar.h"

DeclareClass( BarDaughter, Bar );

BarDaughter bardaughter_use( void * );
BarDaughter bardaughter_use_with_data( void *, int );
BarDaughter bardaughter_use_with_text( void *, const char * text );

/* Virtual function definitions
 */

Virtuals( BarDaughter, Bar )

	void	(* bardaughter_virtual)  ( BarDaughter );

EndOfVirtuals;

#endif  /* BARDAUGHTER_H */
