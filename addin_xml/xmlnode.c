
/* This is a XmlNode class implementation file
 */

#include <string.h>

#include "xmlnode.h"
#include "implement/xml.h"

#include <ooc/exception.h>
#include <ooc/list.h>
#include <ooc/implement/list.h>


/** @class XmlNode
 *  @brief XmlNode class - brief description.
 * 	@see xmlnode.h
 */

/** @file xmlnode.h 
 *  @brief XmlNode class - brief description.
 * XmlNode is a .....
 * @note	This class is a final class, can not be inherited.
 */ 

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( XmlNode, ListNode )

	XmlNodeTypes		type;
	char *				name;
	XmlNode				parent;
	List				children;
	char *				value;

EndOfClassMembers;

Virtuals( XmlNode, ListNode )
	Interface( Xml );
EndOfVirtuals;

InterfaceRegister( XmlNode )
{
	AddInterface( XmlNode, Xml )
};

AllocateClassWithInterface( XmlNode, ListNode );

static void XmlNode_populate( Xml );

static
void
XmlNode_initialize( Class this )
{
	XmlNodeVtable vtab = & XmlNodeVtableInstance;
	
	ooc_init_class( List );
	ooc_init_class( XmlException );

	XmlNode_populate( & vtab->Xml );
}

/* Class finalizing
 */

static
void
XmlNode_finalize( Class this )
{
	/* Release global resources! */
}


/* Constructor
 */

static
void
XmlNode_constructor( XmlNode self, const void * params )
{
	assert( ooc_isInitialized( XmlNode ) );
	
	chain_constructor( XmlNode, self, NULL );
}

static
void
XmlNode_destructor( XmlNode self, XmlNodeVtable vtab )
{
	ooc_delete_and_null( (Object*) & self->children );
	ooc_free_and_null( (void**) & self->name );
	ooc_free_and_null( (void**) & self->value );
}

/* Copy constuctor
 */

static
int
XmlNode_copy( XmlNode self, const XmlNode from )
{
	self->type		= from->type;
	self->name		= ooc_strdup( from->name );
	self->value		= ooc_strdup( from->value );
	self->children	= (List) ooc_duplicate( (Object) from->children );
	
	return OOC_COPY_DONE;
}

/*	=====================================================
	Class member functions
 */

XmlNode
xmlnode_new( )
{
	return ooc_new( XmlNode, NULL );
}

void
xmlnode_set_name( XmlNode self, const char * name )
{
	assert( ooc_isInstanceOf( self, XmlNode ) );

	if( name == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_IMPLEMENTATION ) );
	
	ooc_free( self->name );
	self->name = ooc_strdup( (char*) name );
}

const char *
xmlnode_get_name( XmlNode self )
{
	return self->name;
}

void
xmlnode_set_type( XmlNode self, XmlNodeTypes type )
{
	assert( ooc_isInstanceOf( self, XmlNode ) );

	if( type <= XML_NODE_UNDEFINED || type >= XML_NODE_TYPES_MAX )
		ooc_throw( xmlexception_new( XML_ERROR_IMPLEMENTATION ) );

	self->type = type;
}

XmlNodeTypes
xmlnode_get_type( XmlNode self )
{
	return self->type;
}

void
xmlnode_set_value( XmlNode self, const char * value )
{
	assert( ooc_isInstanceOf( self, XmlNode ) );

	ooc_free( self->value );
	self->value = ooc_strdup( (char*) value );
}

const char *
xmlnode_get_value( XmlNode self )
{
	return self->value;
}

void
xmlnode_add_child( XmlNode self, XmlNode child )
{
	ooc_manage_object( child );

	assert( ooc_isInstanceOf( self, XmlNode ) );
	assert( ooc_isInstanceOf( child, XmlNode ) );

	if( self->children == NULL )
		self->children = list_new_of_nodes( XmlNode, OOC_MANAGE );

	child->parent = self;
	list_append( self->children, ooc_pass( child ) );
}

void
xmlnode_set_children( XmlNode self, List children )
{
	ooc_manage_object( children );

	assert( ooc_isInstanceOf( self, XmlNode ) );
	assert( ooc_isInstanceOf( children, List ) );
	assert( _ooc_isClassOf( list_get_type( children), & XmlNodeClass) );

	ooc_delete_and_null( (Object*) & self->children );
	self->children = ooc_pass( children );
}

List
xmlnode_get_children( XmlNode self )
{
	return self->children;
}

static
int
check_attrib_name( XmlNode child, const char * name )
{
	assert( ooc_isInstanceOf( child, XmlNode ) );

	if( child->type == XML_NODE_ATTR && name != NULL )
		return strcmp( child->name, (char*) name ) == 0;
	else
		return FALSE;
}

const char *
xmlnode_get_attrib( XmlNode self, const char * name )
{
	XmlNode	 found_attrib;

	assert( ooc_isInstanceOf( self, XmlNode ) );

	if( self->type != XML_NODE_ELEMENT )
		ooc_throw( xmlexception_new( XML_ERROR_NO_ATTRIBS ) );

	found_attrib = (XmlNode) list_find_item( self->children, NULL, (list_item_checker) check_attrib_name, (void*) name );

	if( found_attrib != NULL )
	{
		assert( ooc_isInstanceOf( found_attrib, XmlNode ) );
		return found_attrib->value;
	}
	else
		return NULL;
}


/* Xml interface callbacks
 */

static
XmlReadState
xmlnode_read_beginElement( Object _self, XmlParser parser, XmlReadState state, const char * name, XmlAttribs attribs )
{
	XmlNode self = ooc_cast( _self, XmlNode );
	VectorIndex i;

	assert( state == -1 );

	xmlnode_set_type( self, XML_NODE_ELEMENT );
	xmlnode_set_name( self, name );

	for( i = 0; i < xmlattribs_items( attribs ); i++ )
	{
		XmlNode attr = xmlnode_new( self );
		ooc_manage_object( attr );

		xmlnode_set_type( attr, XML_NODE_ATTR );
		xmlnode_set_name( attr, xmlattribs_get_name( attribs, i ) );
		xmlnode_set_value( attr, xmlattribs_get_value( attribs, i ) );

		xmlnode_add_child( self, ooc_pass( attr ) );
	}
	return 0;
}

static
void
xmlnode_read_text( Object _self, XmlParser parser, XmlReadState state, const char * text )
{
	XmlNode self = ooc_cast( _self, XmlNode );

	XmlNode child = xmlnode_new( self );
	ooc_manage_object( child );
	xmlnode_set_type( child, XML_NODE_TEXT );
	xmlnode_set_value( child, text ? text : "" );
	xmlnode_add_child( self, ooc_pass( child ) );
}

static
void
xmlnode_read_comment( Object _self, XmlParser parser, XmlReadState state, const char * comment )
{
	XmlNode self = ooc_cast( _self, XmlNode );

	XmlNode child = xmlnode_new( self );
	ooc_manage_object( child );
	xmlnode_set_type( child, XML_NODE_COMMENT );
	xmlnode_set_value( child, comment ? comment : "" );
	xmlnode_add_child( self, ooc_pass( child ) );
}

static
void
xmlnode_read_child( Object _self, XmlParser xmlp, XmlReadState state, Object child )
{
	ooc_manage_object( child );

	XmlNode self = ooc_cast( _self, XmlNode );

	assert( state == 0 );

	xmlnode_add_child( self, ooc_pass( ooc_cast( child, XmlNode ) ) );
}

XmlNode
xmlnode_parse( XmlParser xmlp )
{
	XmlNode document;

	ooc_init_class( XmlNode );

	document = xmlnode_new();
	{
		ooc_manage_object( document );

		xmlnode_set_type( document, XML_NODE_DOCUMENT );
		xmlnode_set_name( document, "#document" );
		_xmlparser_set_type( xmlp, & XmlNodeClass );
		_xmlparser_set_root( xmlp, (Object) ooc_pass( document ) );
	}
	return (XmlNode) xmlparser_parse( xmlp );
}


static
void
xmlnode_write_children( XmlNode self, XmlWriter xmlw )
{
	assert( ooc_isInstanceOf( self, XmlNode ) );

	if( self->children )
		list_foreach( self->children, (list_item_executor) xmlnode_write, xmlw );
	else if( self->value )
		xml_write_text( xmlw, self->value );
}

void
xmlnode_write( XmlNode self, XmlWriter xmlw )
{
	assert( ooc_isInstanceOf( self, XmlNode ) );
	assert( ooc_isInstanceOf( xmlw, XmlWriter ) );

	switch( self->type )
	{
		case XML_NODE_DOCUMENT	:	xmlnode_write_children( self, xmlw );
									break;
		case XML_NODE_ELEMENT	:	xml_write_begin_element( xmlw, self->name );
									xmlnode_write_children( self, xmlw );
									xml_write_end_element( xmlw );
									break;
		case XML_NODE_ATTR		:	xml_write_attribute( xmlw, self->name, self->value );
									break;
		case XML_NODE_TEXT		:	xml_write_text( xmlw, self->value );
									break;
		case XML_NODE_CDATA		:	/* TODO: CDATA write implementation */
									break;
		case XML_NODE_COMMENT	:	xml_write_comment( xmlw, self->value );
									break;
		default : ooc_throw( xmlexception_new( XML_ERROR_IMPLEMENTATION ) );
	}
}

static
void
XmlNode_populate( Xml xml )
{
	xml->on_read_beginElement	= xmlnode_read_beginElement;
	xml->on_read_comment		= xmlnode_read_comment;
	xml->on_read_text			= xmlnode_read_text;
	xml->on_read_child			= xmlnode_read_child;

	xml->on_read_endElement 	= NULL;

	xml->on_write_begin			= (void	(*)( Object, XmlWriter )) xmlnode_write;
	xml->on_write_attributes	= NULL;
	xml->on_write_data			= NULL;
	xml->on_write_end			= NULL;
}

