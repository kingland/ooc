
/* This is a Tonic class implementation file
 */

#include "tonic.h"
#include "implement/tonic.h"

#include <ooc/exception.h>

/* Allocating the class description table and the vtable
 */

AllocateClass( Tonic, Base );

/* Class initializing
 */

static
void
Tonic_initialize( Class this )
{
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static
void
Tonic_finalize( Class this )
{
}
#endif


/* Constructor
 */

static
void
Tonic_constructor( Tonic self, const void * params )
{
	assert( ooc_isInitialized( Tonic ) );
	
	chain_constructor( Tonic, self, NULL );

}

/* Destructor
 */

static
void
Tonic_destructor( Tonic self, TonicVtable vtab )
{
}

/* Copy constuctor
 */

static
int
Tonic_copy( Tonic self, const Tonic from )
{
	return OOC_COPY_DONE;
}

/*	=====================================================
	Class member functions
 */


const char *
tonic_name( Tonic self )
{
	assert( ooc_isInstanceOf( self, Tonic ) );

	return	ooc_get_type( (Object) self )->c.type.name;
}

