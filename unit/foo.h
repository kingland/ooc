
#ifndef FOO_H
#define FOO_H

#include "../libs/ooc.h"

DeclareClass( Foo, Base );

Foo foo_new( void );
Foo foo_new_with_data( int );
Foo foo_new_with_text( char * text );
Foo foo_new_with_const_text( const char * text );

void			foo_set_data( Foo self, int data );
int				foo_get_data( Foo self );

void			foo_add_text( Foo self, char * text );
const char *	foo_get_text( Foo self );

/* Virtual function definitions
 */

Virtuals( Foo, Base )

	void	(* foo_virtual)  ( Foo );

EndOfVirtuals;

#endif  /* FOO_H */
