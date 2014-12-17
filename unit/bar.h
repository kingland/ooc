
#ifndef BAR_H
#define BAR_H

#include "../libs/ooc.h"

DeclareClass( Bar, Base );

Bar bar_new( void );
Bar bar_new_with_data( int );
Bar bar_new_with_text( char * text );
Bar bar_new_with_const_text( const char * text );

void			bar_set_data( Bar self, int data );
int				bar_get_data( Bar self );

void			bar_add_text( Bar self, char * text );
const char *	bar_get_text( Bar self );

/* Virtual function definitions
 */

Virtuals( Bar, Base )

	void	(* bar_virtual)  ( Bar );

EndOfVirtuals;

#endif  /* BAR_H */
