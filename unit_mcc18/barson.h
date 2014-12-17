
#ifndef BARSON_H
#define BARSON_H

#include "../libs/ooc.h"

#include "bar.h"

DeclareClass( BarSon, Bar );

BarSon barson_use( void * );
BarSon barson_use_with_data( void *, int );
BarSon barson_use_with_text( void *, const char * text );

/* Virtual function definitions
 */

Virtuals( BarSon, Bar )

	void	(* barson_virtual)  ( BarSon );

EndOfVirtuals;

#endif  /* BARSON_H */
