
#ifndef XMLNODE_H
#define XMLNODE_H

#include <ooc/list.h>

#include "xml.h"

DeclareClass( XmlNode, ListNode );

XmlNode		xmlnode_new( );

void		xmlnode_write( XmlNode, XmlWriter );
XmlNode		xmlnode_parse( XmlParser );


typedef
enum _XmlNodeTypes
{
	XML_NODE_UNDEFINED = 0,

	XML_NODE_DOCUMENT,
	XML_NODE_ELEMENT,
	XML_NODE_ATTR,
	XML_NODE_TEXT,
	XML_NODE_CDATA,
	XML_NODE_COMMENT,

	XML_NODE_TYPES_MAX
} XmlNodeTypes;

void			xmlnode_set_name( XmlNode, const char * );
const char *	xmlnode_get_name( XmlNode );
void			xmlnode_set_type( XmlNode, XmlNodeTypes );
XmlNodeTypes	xmlnode_get_type( XmlNode );
void			xmlnode_set_value( XmlNode, const char * );
const char *	xmlnode_get_value( XmlNode );
void			xmlnode_add_child( XmlNode, XmlNode child );
void			xmlnode_set_children( XmlNode, List children );
List			xmlnode_get_children( XmlNode );
const char *	xmlnode_get_attrib( XmlNode, const char * name );

#endif  /* XMLNODE_H */
