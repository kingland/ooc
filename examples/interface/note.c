
/* This is a Note class implementation file
 */

#include "note.h"
#include "implement/note.h"

#include <ooc/exception.h>

#include <stdio.h>

#include "factory.h"

InterfaceRegister( Note )
{
	AddInterface( Note, Serializable )
};

AllocateClassWithInterface( Note, Base );

/* Class virtual function prototypes
 */

static void note_serialize( Note, int indent );

/* Class initializing
 */

static
void
Note_initialize( Class this )
{
	NoteVtable vtab = & NoteVtableInstance;
	
	vtab->Serializable.serialize = 	(void (*)(Object, int)) note_serialize;
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static
void
Note_finalize( Class this )
{
	/* Release global resources! */
}
#endif


/* Constructor
 */

static
void
Note_constructor( Note self, const void * params )
{
	assert( ooc_isInitialized( Note ) );
	
	chain_constructor( Note, self, NULL );

	self->owner =	get_an_owner();
	self->date  =	get_a_date();
	self->text  =	get_a_text();
}

/* Destructor
 */

static
void
Note_destructor( Note self, NoteVtable vtab )
{
}

/* Copy constuctor
 */

static
int
Note_copy( Note self, const Note from )
{
	return OOC_COPY_DEFAULT;
}

/*	=====================================================
	Class member functions
 */


Note
note_new( void )
{
	ooc_init_class( Note );
		
	return ooc_new( Note, NULL );
}


static
void
note_serialize( Note self, int indent )
{
	assert( ooc_isInstanceOf( self, Note ) );

	printf( "%*s<Note date=\"%s\" owner=\"%s\">\n", indent, "", self->date, self->owner );
	printf( "%*s<Text>\n", indent+1, "" );
	printf( "%*s%s\n", indent+2, "", self->text );
	printf( "%*s<\\Text>\n", indent+1, "" );
	printf( "%*s<\\Note>\n", indent, "" );
}

