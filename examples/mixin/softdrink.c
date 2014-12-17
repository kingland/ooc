
/* This is a SoftDrink class implementation file
 */

#include "softdrink.h"
#include "implement/softdrink.h"

#include <ooc/exception.h>

/* Allocating the class description table and the vtable
 */

AllocateClass( SoftDrink, Tonic );

/* Class initializing
 */

static
void
SoftDrink_initialize( Class this )
{
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static
void
SoftDrink_finalize( Class this )
{
}
#endif


/* Constructor
 */

static
void
SoftDrink_constructor( SoftDrink self, const void * params )
{
	assert( ooc_isInitialized( SoftDrink ) );
	
	chain_constructor( SoftDrink, self, NULL );

	self->size = 2;
}

/* Destructor
 */

static
void
SoftDrink_destructor( SoftDrink self, SoftDrinkVtable vtab )
{
}

/* Copy constuctor
 */

static
int
SoftDrink_copy( SoftDrink self, const SoftDrink from )
{
	return OOC_COPY_DEFAULT;
}

/*	=====================================================
	Class member functions
 */


SoftDrink
softdrink_new( void )
{
	ooc_init_class( SoftDrink );
		
	return (SoftDrink) ooc_new( SoftDrink, NULL );
}
