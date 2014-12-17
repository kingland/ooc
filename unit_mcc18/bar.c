
/* This is a Bar class implementation file
 */

#include "../libs/exception.h"

#include "bar.h"
#include "implement/bar.h"

/** @class Bar
 *  @brief Bar class - brief description.
 * 	@see bar.h
 */

/** @file bar.h 
 *  @brief Bar class - brief description.
 * Bar is a .....
 */ 

/* Allocating the class description table and the vtable
 */

AllocateClass( Bar, Base );

/* Class virtual function prototypes
 */

static
void
_bar_virtual( Bar self )
{
	return;
}


/* Class initializing
 */

static
void
Bar_initialize( Class this )
{
	BarVtable vtab = (BarVtable) this->c.vtable;
	
	vtab->bar_virtual	= 	_bar_virtual;

	// ooc_init_class( !!! Classes that are USED by Bar !!! ); Remove if you initialize it other place!
	
	/* Allocate global resources here */
}

/* Class finalizing
 */
#ifndef OOC_NO_FINALIZE

static
void
Bar_finalize( Class this )
{
	/* Release global resources! */
}
#endif

/* Constructor
 */

static
void
Bar_constructor( Bar self, const void * params )
{
	assert( ooc_isInitialized( Bar ) );
	
	chain_constructor( Bar, self, NULL );

}

/* Destructor
 */

static
void
Bar_destructor( Bar self, BarVtable vtab )
{
	self->text = NULL;
}

/* Copy constuctor
 */

static
int
Bar_copy( Bar self, const Bar from )
{
	/* makes the default object copying (bit-by-bit) */
	return OOC_COPY_DEFAULT;
}

/*	=====================================================
	Class member functions
 */

Bar
bar_use( void * bar )
{
	ooc_init_class( Bar );
		
	ooc_use( bar, Bar, NULL );

	return (Bar) bar;
}

Bar
bar_use_with_data( void * bar, int data )
{
	bar_use( bar );

	bar_set_data( (Bar) bar, data );
	
	return (Bar) bar;
}

Bar
bar_use_with_text( void * bar, const char * text )
{
	bar_use( bar );
	
	bar_add_text( bar, text );
	
	return (Bar) bar;
}

void
bar_set_data( Bar self, int data )
{
	self->data = data;
}

int
bar_get_data( Bar self )
{
	return self->data;
}

void
bar_add_text( Bar self, const char * text )
{
	self->text = text;	
}

const char *
bar_get_text( Bar self )
{
	return self->text;
}
