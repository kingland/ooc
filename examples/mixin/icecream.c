
/* This is a IceCream class implementation file
 */

#include "icecream.h"
#include "implement/icecream.h"

#include <ooc/exception.h>

/* Allocating the class description table and the vtable
 */

InterfaceRegister( IceCream )
{
	AddMixin( IceCream, Flavour )
};

AllocateClassWithInterface( IceCream, Tonic );

/* Class initializing
 */

static
void
IceCream_initialize( Class this )
{
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static
void
IceCream_finalize( Class this )
{
}
#endif


/* Constructor
 */

static
void
IceCream_constructor( IceCream self, const void * flavour )
{
	assert( ooc_isInitialized( IceCream ) );
	
	chain_constructor( IceCream, self, NULL );

	if( flavour )
		IceCreamVirtual( self )->Flavour.set( (Object) self, flavour );
}

/* Destructor
 */

static
void
IceCream_destructor( IceCream self, IceCreamVtable vtab )
{
}

/* Copy constuctor
 */

static
int
IceCream_copy( IceCream self, const IceCream from )
{
	/* makes the default object copying (bit-by-bit) */
	return OOC_COPY_DEFAULT;
}

/*	=====================================================
	Class member functions
 */


IceCream
icecream_new( const char * flavour )
{
	ooc_init_class( IceCream );
		
	return ooc_new( IceCream, flavour );
}
