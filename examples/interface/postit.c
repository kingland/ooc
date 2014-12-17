
/* This is a PostIt class implementation file
 */

#include "postit.h"
#include "implement/postit.h"

#include <ooc/exception.h>

#include <stdio.h>

#include "factory.h"

/* Allocating the class description table and the vtable
 */

AllocateClass( PostIt, Note );

/* Class virtual function prototypes
 */

static void postit_serialize( PostIt self, int indent );

/* Class initializing
 */

static
void
PostIt_initialize( Class this )
{
	PostItVtable vtab = & PostItVtableInstance;
	
	vtab->Note.Serializable.serialize	= (void (*)(Object, int)) postit_serialize;
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static
void
PostIt_finalize( Class this )
{
}
#endif


/* Constructor
 */

static
void
PostIt_constructor( PostIt self, const void * params )
{
	assert( ooc_isInitialized( PostIt ) );
	
	chain_constructor( PostIt, self, NULL );

	self->color = get_a_color();
}

/* Destructor
 */

static
void
PostIt_destructor( PostIt self, PostItVtable vtab )
{
}

/* Copy constuctor
 */

static
int
PostIt_copy( PostIt self, const PostIt from )
{
	/* makes the default object copying (bit-by-bit) */
	return OOC_COPY_DEFAULT;
}

PostIt
postit_new( void )
{
	ooc_init_class( PostIt );
		
	return (PostIt) ooc_new( PostIt, NULL );
}

/* Example for calling the parent class's interface method:
*/

static
void
postit_serialize( PostIt self, int indent )
{
	assert( ooc_isInstanceOf( self, PostIt ) );

	printf( "%*s<PostIt color=\"%s\">\n", indent, "", self->color );

	PostItParentVirtual( self )->Serializable.serialize( (Object) self, indent + 1 );

	printf( "%*s<\\PostIt>\n", indent, "" );
}
