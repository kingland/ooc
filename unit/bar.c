
/* This is a Bar class implementation file
 */

#include "implement/bar.h"

#include "../libs/exception.h"

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
	BarVtable vtab = & BarVtableInstance;
	
	vtab->bar_virtual	= 	_bar_virtual;

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
	ooc_free_and_null( (void**) & self->text );
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
bar_new( void )
{
	ooc_init_class( Bar );
		
	return (Bar) ooc_new( Bar, NULL );
}

Bar
bar_new_with_data( int data )
{
	Bar bar = bar_new();
	
	bar_set_data( bar, data );
	
	return bar;
}

Bar
bar_new_with_text( char * text )
{
	Bar bar;
	
	ooc_manage( text, (ooc_destroyer) ooc_free );
	
	bar = bar_new();
	
	bar_add_text( bar, ooc_pass( text ) );
	
	return bar;
}

Bar
bar_new_with_const_text( const char * text )
{
	Bar bar = bar_new();
	
	{
	ooc_manage_object( bar );
	bar_add_text( bar, ooc_strdup( text ) );
	return ooc_pass( bar );
	}
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
bar_add_text( Bar self, char * text )
{
	ooc_free( self->text );
	self->text = text;	
}

const char *
bar_get_text( Bar self )
{
	return self->text;
}
