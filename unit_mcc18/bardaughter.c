
/* This is a BarDaughter class implementation file
 */

#include "bardaughter.h"
#include "implement/bardaughter.h"

#include "../libs/exception.h"

/** @class BarDaughter
 *  @brief BarDaughter class - brief description.
 * 	@see bardaughter.h
 */

/** @file bardaughter.h 
 *  @brief BarDaughter class - brief description.
 * BarDaughter is a .....
 */ 

/* Allocating the class description table and the vtable
 */

AllocateClass( BarDaughter, Bar );

/* Class virtual function prototypes
 */

static
void
_bardaughter_virtual( BarDaughter self )
{
	return;
}


/* Class initializing
 */

static
void
BarDaughter_initialize( Class this )
{
	BarDaughterVtable vtab = (BarDaughterVtable) this->c.vtable;
	
	vtab->bardaughter_virtual	= 	_bardaughter_virtual;

	// ooc_init_class( !!! Classes that are USED by BarDaughter !!! ); Remove if you initialize it other place!
	
	/* Allocate global resources here */
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static
void
BarDaughter_finalize( Class this )
{
	/* Release global resources! */
}
#endif

/* Constructor
 */

static
void
BarDaughter_constructor( BarDaughter self, const void * params )
{
	assert( ooc_isInitialized( BarDaughter ) );
	
	chain_constructor( BarDaughter, self, NULL );

}

/* Destructor
 */

static
void
BarDaughter_destructor( BarDaughter self, BarDaughterVtable vtab )
{
}

/* Copy constuctor
 */

static
int
BarDaughter_copy( BarDaughter self, const BarDaughter from )
{
	/* makes the default object copying (bit-by-bit) */
	return OOC_COPY_DEFAULT;
}

/*	=====================================================
	Class member functions
 */


BarDaughter
bardaughter_use( void * self )
{
	ooc_init_class( BarDaughter );
	
	ooc_use( self, BarDaughter, NULL );

	return (BarDaughter) self;
}

typedef struct BarObject * Bar;

BarDaughter
bardaughter_use_with_data( void * self, int data )
{
	BarDaughter bardaughter = bardaughter_use( self );
	
	bar_set_data( (Bar) bardaughter, data );
	
	return bardaughter;
}

BarDaughter
bardaughter_use_with_text( void * self, const char * text )
{
	BarDaughter bardaughter;
	
	bardaughter = bardaughter_use( self );
	
	bar_add_text( (Bar) bardaughter, text );
	
	return bardaughter;
}
