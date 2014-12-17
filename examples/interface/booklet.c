
/* This is a Booklet class implementation file
 */

#include <stdio.h>

#include <ooc/exception.h>

#include "booklet.h"
#include "implement/booklet.h"

#include "note.h"

#include "factory.h"

/* Allocating the class description table and the vtable
 */

InterfaceRegister( Booklet )
{
	AddInterface( Booklet, Serializable )
};

AllocateClassWithInterface( Booklet, Base );

/* Class virtual function prototypes
 */

static void booklet_serialize( Booklet self, int indent );

/* Class initializing
 */

static
void
Booklet_initialize( Class this )
{
	BookletVtable vtab = & BookletVtableInstance;
	
	ooc_init_class( Vector );

	vtab->Serializable.serialize	= 	(void (*)(Object, int))		booklet_serialize;
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static
void
Booklet_finalize( Class this )
{
}
#endif


/* Constructor
 */

static
void
Booklet_constructor( Booklet self, const void * params )
{
	VectorIndex i;

	assert( ooc_isInitialized( Booklet ) );
	
	chain_constructor( Booklet, self, NULL );

	self->owner	= get_an_owner();
	self->date	= get_a_date();

	i = rand_index( 5 ) + 2;

	self->notes = vector_new_type( i, Note, OOC_MANAGE );

	while( i-- )
		vector_push_back( self->notes, note_new() );
}

/* Destructor
 */

static
void
Booklet_destructor( Booklet self, BookletVtable vtab )
{
	ooc_delete_and_null( (Object *) & self->notes );
}

/* Copy constuctor
 */

static
int
Booklet_copy( Booklet self, const Booklet from )
{
	self->date  = from->date;
	self->owner = from->owner;
	self->notes = (Vector) ooc_duplicate( (Object) from->notes );

	return OOC_COPY_DONE;
}

/*	=====================================================
	Class member functions
 */


Booklet
booklet_new( void )
{
	ooc_init_class( Booklet );

	return ooc_new( Booklet, NULL );
}

/* Example for calling the same interface for children of known types:
*/

static
void
booklet_put_notes( Note note, int * indent )
{
	assert( ooc_isInstanceOf( note, Note ) );

	NoteVirtual( note )->Serializable.serialize( (Object) note, *indent+1 );
}

static
void
booklet_serialize( Booklet self, int indent )
{
	assert( ooc_isInstanceOf( self, Booklet ) );

	printf( "%*s<Booklet date=\"%s\" owner=\"%s\" pages=\"%d\">\n", indent, "", self->date, self->owner, vector_items( self->notes ) );
	
	vector_foreach( self->notes, (vector_item_executor) booklet_put_notes, & indent );

	printf( "%*s<\\Booklet>\n", indent, "" );
}

