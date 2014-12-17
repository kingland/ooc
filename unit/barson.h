
#ifndef BARSON_H
#define BARSON_H

#include "../libs/ooc.h"

#include "bar.h"

DeclareClass( BarSon, Bar );

BarSon barson_new( void );
BarSon barson_new_with_data( int );
BarSon barson_new_with_text( char * text );
BarSon barson_new_with_const_text( const char * text );

/* Virtual function definitions
 */

Virtuals( BarSon, Bar )

	void	(* barson_virtual)  ( BarSon );

EndOfVirtuals;

#endif  /* BARSON_H */
