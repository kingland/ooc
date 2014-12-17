
/* This is a BarSon class implementation file
 */

#include "implement/barson.h"

#include "../libs/exception.h"

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
	BarSonVtable vtab = & BarSonVtableInstance;
	
	vtab->barson_virtual	= 	_barson_virtual;

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
barson_new( void )
{
	ooc_init_class( BarSon );
		
	return (BarSon) ooc_new( BarSon, NULL );
}

BarSon
barson_new_with_data( int data )
{
	BarSon barson = barson_new();
	
	bar_set_data( (Bar) barson, data );
	
	return barson;
}

BarSon
barson_new_with_text( char * text )
{
	BarSon barson;
	
	ooc_manage( text, (ooc_destroyer) ooc_free );
	
	barson = barson_new();
	
	bar_add_text( (Bar) barson, ooc_pass( text ) );
	
	return barson;
}

BarSon
barson_new_with_const_text( const char * text )
{
	BarSon barson = barson_new();
	
	{
	ooc_manage_object( barson );
	bar_add_text( (Bar) barson, ooc_strdup( text ) );
	return ooc_pass( barson );
	}
}
