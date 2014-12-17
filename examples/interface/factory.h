#ifndef FACTORY_H
#define FACTORY_H

#include <stdlib.h>
#include <ooc/ooc.h>

const char *	get_an_owner();
const char *	get_a_date();
const char *	get_a_text();
const char *	get_a_color();

size_t			rand_index( int upper_limit );


DeclareClass( GlassOfWine, Base );
DeclareClass( Mouse, Base );
DeclareClass( Keyboard, Base );

#endif /* FACTORY_H */
