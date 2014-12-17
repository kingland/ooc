
/* This is a Desktop class implementation file
 */

#include <stdio.h>
#include <stdlib.h>

#include <ooc/exception.h>
#include <ooc/vector.h>

#include "desktop.h"

#include "note.h"
#include "postit.h"
#include "booklet.h"

#include "factory.h"

ClassMembers( Desktop, Base )

	const char *	owner;
	Vector			things;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

InterfaceRegister( Desktop )
{
	AddInterface( Desktop, Serializable )
};

AllocateClassWithInterface( Desktop, Base );

static void desktop_serialize( Desktop self, int indent );

static
void
Desktop_initialize( Class this )
{
	DesktopVtable vtab = & DesktopVtableInstance;
	
	vtab->Serializable.serialize	= 	(void (*)(Object, int)) desktop_serialize;

	ooc_init_class( Vector );

	ooc_init_class( Mouse );
	ooc_init_class( Keyboard );
	ooc_init_class( GlassOfWine );
}

static
void
Desktop_finalize( Class this )
{
}

static
void
Desktop_constructor( Desktop self, const void * params )
{
	size_t	items, i;

	assert( ooc_isInitialized( Desktop ) );
	
	chain_constructor( Desktop, self, NULL );

	self->owner = get_an_owner();

	items = rand_index( 6 ) + 3;

	self->things = vector_new_type( items, Base, OOC_MANAGE );

	for( i=0; i<items; i++ )
	{
		switch( rand_index( 6 ) )
		{
			default :
			case 0 :	vector_push_back( self->things, booklet_new() );
						break;
			case 1 :	vector_push_back( self->things, postit_new() );
						break;
			case 2 :	vector_push_back( self->things, note_new() );
						break;
			case 3 :	vector_push_back( self->things, ooc_new( Mouse, NULL ) );
						break;
			case 4 :	vector_push_back( self->things, ooc_new( Keyboard, NULL ) );
						break;
			case 5 :	vector_push_back( self->things, ooc_new( GlassOfWine, NULL ) );
						break;
		}
	}	
}

static
void
Desktop_destructor( Desktop self, DesktopVtable vtab )
{
	ooc_delete_and_null( (Object *) & self->things );
}

/* Copy constuctor
 */

static
int
Desktop_copy( Desktop self, const Desktop from )
{
	self->things = (Vector) ooc_duplicate( (Object) from->things );

	return OOC_COPY_DONE;
}

Desktop
desktop_new( void )
{
	ooc_init_class( Desktop );
		
	return (Desktop) ooc_new( Desktop, NULL );
}

/* Example for calling the an interface for children of unknown types:
*/

static
void
desktop_put_thing( Object thing, int * indent )
{
	Serializable serializable;

	assert( ooc_isInstanceOf( thing, Base ) );

	serializable = ooc_get_interface( thing, Serializable );

	if( serializable )
		serializable->serialize( thing, *indent+1 );
}


static
void
desktop_serialize( Desktop self, int indent )
{
	assert( ooc_isInstanceOf( self, Desktop ) );

	printf( "%*s<Desktop owner=\"%s\" things=\"%d\">\n", indent, "", self->owner, vector_items( self->things ) );
	
	vector_foreach( self->things, (vector_item_executor) desktop_put_thing, & indent );

	printf( "%*s<\\Desktop>\n", indent, "" );
}

