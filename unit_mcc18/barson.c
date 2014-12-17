
/* This is a BarSon class implementation file
 */

#include "../libs/exception.h"

#include "barson.h"
#include "implement/barson.h"

/** @class BarSon
 *  @brief BarSon class - brief description.
 * 	@see barson.h
 */

/** @file barson.h 
 *  @brief BarSon class - brief description.
 * BarSon is a .....
 */ 

/* Allocating the class description table and the vtable
 */

AllocateClass( BarSon, Bar );

/* Class virtual function prototypes
 */

static
void
_barson_virtual( BarSon self )
{
	return;
}


/* Class initializing
 */

static
void
BarSon_initialize( Class this )
{
	BarSonVtable vtab = (BarSonVtable) this->c.vtable;
	
	vtab->barson_virtual	= 	_barson_virtual;

	// ooc_init_class( !!! Classes that are USED by BarSon !!! ); Remove if you initialize it other place!
	
	/* Allocate global resources here */
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static
void
BarSon_finalize( Class this )
{
	/* Release global resources! */
}
#endif

/* Constructor
 */

static
void
BarSon_constructor( BarSon self, const void * params )
{
	assert( ooc_isInitialized( BarSon ) );
	
	chain_constructor( BarSon, self, NULL );

}

/* Destructor
 */

static
void
BarSon_destructor( BarSon self, BarSonVtable vtab )
{
}

/* Copy constuctor
 */

static
int
BarSon_copy( BarSon self, const BarSon from )
{
	/* makes the default object copying (bit-by-bit) */
	return OOC_COPY_DEFAULT;
}

/*	=====================================================
	Class member functions
 */

BarSon
barson_use( void * barson )
{
	ooc_init_class( BarSon );
		
	ooc_use( barson, BarSon, NULL );

	return (BarSon) barson;	
}

BarSon
barson_use_with_data( void * barson, int data )
{
	barson_use( barson );
	
	bar_set_data( barson, data );
	
	return (BarSon) barson;
}

BarSon
barson_use_with_text( void * barson, const char * text )
{
	barson_use( barson );
	
	bar_add_text( barson, text );
	
	return (BarSon) barson;
}
