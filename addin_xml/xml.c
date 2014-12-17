
/* This is a XmlBase class implementation file
 */

#include "xml.h"
#include "implement/xml.h"

#include <ooc/implement/exception.h>
#include <ooc/implement/vector.h>

#include <string.h>

AllocateInterface( Xml );

AllocateClass( XmlBase, Base );

static void XmlBase_initialize( Class this ) {}

#ifndef OOC_NO_FINALIZE
static void XmlBase_finalize( Class this ) {}
#endif

static void XmlBase_constructor( XmlBase self, const void * params ) {}
static void XmlBase_destructor( XmlBase self, XmlBaseVtable vtab ) {}
static int	XmlBase_copy( XmlBase self, const XmlBase from ) { 	return OOC_COPY_DEFAULT; }

/** @class XmlException
 *  @brief XmlException class - brief description.
 * 	@see xmlexception.h
 */

/** @file xmlexception.h 
 *  @brief XmlException class - brief description.
 * XmlException is a .....
 * @note	This class is a final class, can not be inherited.
 */ 

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( XmlException, Exception )

EndOfClassMembers;

Virtuals( XmlException, Exception )
EndOfVirtuals;

AllocateClass( XmlException, Exception );

static void XmlException_initialize( Class this ) {}
static void XmlException_finalize( Class this ) {}

static
void
XmlException_constructor( XmlException self, const void * params )
{
	assert( ooc_isInitialized( XmlException ) );
	
	chain_constructor( XmlException, self, NULL );

    if( params )
        self->Exception.user_code = * ( (int*) params );
}

static void XmlException_destructor( XmlException self, XmlExceptionVtable vtab ) {}

static
int
XmlException_copy( XmlException self, const XmlException from )
{
	return OOC_COPY_DEFAULT;
}

Exception
xmlexception_new( enum XmlErrorCodes error )
{
	ooc_init_class( XmlException );
		
	return (Exception) ooc_new( XmlException, & error );
}

/** @class XmlAttribs
 *  @brief XmlAttribs class - brief description.
 * 	@see xml.h
 */

/** @brief XmlAttribs class - brief description.
 * XmlAttribs is a .....
 * @note	This class is a final class, can not be inherited.
 */ 

ClassMembers( XmlAttribs, Vector )
/* on even position are names,
   on odd positions are values */
EndOfClassMembers;

Virtuals( XmlAttribs, Vector )
EndOfVirtuals;

AllocateClass( XmlAttribs, Vector );

static
void
XmlAttribs_initialize( Class this )
{
}

static
void
XmlAttribs_finalize( Class this )
{
}

#define XML_ATTRIBS_CHUNK_SIZE 4

static
void
XmlAttribs_constructor( XmlAttribs self, const void * params )
{
	struct	VectorConstructorParams vp;

	assert( ooc_isInitialized( XmlAttribs ) );
	
	vp.destroy = ooc_free;
	vp.size = XML_ATTRIBS_CHUNK_SIZE * 2;
	vp.type = NULL;

	chain_constructor( XmlAttribs, self, &vp );
}

static
void
XmlAttribs_destructor( XmlAttribs self, XmlAttribsVtable vtab )
{
}

static
int
XmlAttribs_copy( XmlAttribs self, const XmlAttribs from )
{
	/* TODO: will be never executed :-( because Vector_copy executes first, and throws
	 * an exception, because of the untyped Vector (it is not copyable).
	 */
	VectorIndex i;

	XmlAttribs_constructor( self, NULL );

	for( i=0; i<vector_items( (Vector) from ); i++ )
		vector_push_back( (Vector) self, vector_get_item( (Vector) from, i ) );

	return OOC_COPY_DONE;
}

XmlAttribs
xmlattribs_new( void )
{
	return ooc_new( XmlAttribs, NULL );
}

void
xmlattribs_append( XmlAttribs self, const char * name, const char * value )
{
	assert( ooc_isInstanceOf( self, XmlAttribs ) );
	assert( (vector_items( (Vector) self ) & 1) == 0 );

	vector_push_back( (Vector) self, ooc_strdup( (char*) name ) );
	vector_push_back( (Vector) self, ooc_strdup( (char*) value ) );
}

VectorIndex
xmlattribs_items( XmlAttribs self )
{
	assert( ooc_isInstanceOf( self, XmlAttribs ) );

	return vector_items( (Vector) self ) / 2;
}

const char *
xmlattribs_get_name( XmlAttribs self, VectorIndex i )
{
	assert( ooc_isInstanceOf( self, XmlAttribs ) );

	return (const char *) vector_get_item( (Vector) self,  2*i );
}

const char *
xmlattribs_get_value( XmlAttribs self, VectorIndex i )
{
	assert( ooc_isInstanceOf( self, XmlAttribs ) );

	return (const char *) vector_get_item( (Vector) self,  2*i+1 );
}

const char *
xmlattribs_get_value_by_name( XmlAttribs self, const char * name )
{
	VectorIndex i;

	assert( ooc_isInstanceOf( self, XmlAttribs ) );

	if( name )
		for( i = 0; i < xmlattribs_items( self); i++ )
		{
			const char * i_name;

			i_name = (const char *) vector_get_item( (Vector) self,  2*i );
			if( i_name && name && strcmp( i_name, name ) )
				return (const char *) vector_get_item( (Vector) self,  2*i+1 );
		};
	return NULL;
}


