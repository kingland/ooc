
/* This is a BarDaughter class implementation file
 */

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
	BarDaughterVtable vtab = & BarDaughterVtableInstance;
	
	vtab->bardaughter_virtual	= 	_bardaughter_virtual;

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
bardaughter_new( void )
{
	ooc_init_class( BarDaughter );
		
	return (BarDaughter) ooc_new( BarDaughter, NULL );
}

BarDaughter
bardaughter_new_with_data( int data )
{
	BarDaughter bardaughter = bardaughter_new();
	
	bar_set_data( (Bar) bardaughter, data );
	
	return bardaughter;
}

BarDaughter
bardaughter_new_with_text( char * text )
{
	BarDaughter bardaughter;
	
	ooc_manage( text, (ooc_destroyer) ooc_free );
	
	bardaughter = bardaughter_new();
	
	bar_add_text( (Bar) bardaughter, ooc_pass( text ) );
	
	return bardaughter;
}

BarDaughter
bardaughter_new_with_const_text( const char * text )
{
	BarDaughter bardaughter = bardaughter_new();
	
	{
	ooc_manage_object( bardaughter );
	bar_add_text( (Bar) bardaughter, ooc_strdup( text ) );
	return ooc_pass( bardaughter );
	}
}
