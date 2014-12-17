
/* This is a Sample class implementation file
 */

#include "sample.h"
#include "implement/sample.h"

#include <ooc/exception.h>

/** @class Sample
 *  @brief Sample class - brief description.
 * 	@see sample.h
 */

/** @file sample.h 
 *  @brief Sample class - brief description.
 * Sample is a .....
 */ 

/* Allocating the class description table and the vtable
 */

AllocateClass( Sample, Base );

/* Class virtual function prototypes
 */

static
void
_sample_virtual( Sample self )
{
	return;
}


/* Class initializing
 */

static
void
Sample_initialize( Class this )
{
	SampleVtable vtab = & SampleVtableInstance;
	
	vtab->sample_virtual	= 	_sample_virtual;

	ooc_init_class( !!! Classes that are USED by Sample !!! ); Remove if you initialize it other place!
	
	/* Allocate global resources here */
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static
void
Sample_finalize( Class this )
{
	/* Release global resources! */
}
#endif


/* Constructor
 */

static
void
Sample_constructor( Sample self, const void * params )
{
	assert( ooc_isInitialized( Sample ) );
	
	chain_constructor( Sample, self, NULL ); !!! TODO !!! change the third parameter as required! 

}

/* Destructor
 */

static
void
Sample_destructor( Sample self, SampleVtable vtab )
{
}

/* Copy constuctor
 */

static
int
Sample_copy( Sample self, const Sample from )
{
	/* makes the default object copying (bit-by-bit) */
	return OOC_COPY_DEFAULT;
	
	/* Copies data by hand */
	self->data = from->data;
	...
	return OOC_COPY_DONE;
	
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*	=====================================================
	Class member functions
 */


Sample
sample_new( void )
{
	ooc_init_class( Sample ); !!! TODO !!! You may want to remove the initialization from here ...
		
	return (Sample) ooc_new( Sample, NULL );
}


void
sample_methods( Sample self, int )
{
	
}
