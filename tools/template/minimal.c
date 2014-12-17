
/* This is a Sample class implementation file
 */

#include "sample.h"

#include <ooc/exception.h>

#include "implement/base.h" !!! TODO !!! Remove if the class has Base as parent!



/** @class Sample
 *  @brief Sample class - brief description.
 * 	@see sample.h
 */

/** @file sample.h 
 *  @brief Sample class - brief description.
 * Sample is a .....
 */ 

DeclareClass( Sample, Base );

ClassMembers( Sample, Base )
EndOfClassMembers;

Virtuals( Sample, Base )
EndOfVirtuals;

AllocateClass( Sample, Base );

static	void	Sample_initialize( Class this ) {}
static	void	Sample_finalize( Class this ) {}

static	void	Sample_constructor( Sample self, const void * params )
{
	assert( ooc_isInitialized( Sample ) );
	
	chain_constructor( Sample, self, NULL ); !!! TODO !!! change the third parameter as required! 
}

static	void	Sample_destructor( Sample self, SampleVtable vtab ) {}
static	int	Sample_copy( Sample self, const Sample from ) { return OOC_COPY_DEFAULT; }

