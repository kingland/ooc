
#ifndef OOC_XML_H
#define OOC_XML_H

#include <stdio.h>
#include <string.h>

#include <ooc/ooc.h>
#include <ooc/exception.h>
#include <ooc/vector.h>

DeclareClass( XmlBase, Base );

#include "xmlwriter.h"
#include "xmlparser.h"

typedef int XmlReadState;

DeclareClass( XmlAttribs, Base );

XmlAttribs		xmlattribs_new( void );
void			xmlattribs_append( XmlAttribs, const char * name, const char * value );
VectorIndex		xmlattribs_items( XmlAttribs );
const char *	xmlattribs_get_name( XmlAttribs, VectorIndex );
const char *	xmlattribs_get_value( XmlAttribs, VectorIndex );
const char *	xmlattribs_get_value_by_name( XmlAttribs, const char * name );

DeclareInterface( Xml )

	/* XmlWriter callback methods */
	void			(* on_write_begin )		( Object, XmlWriter );
	void 			(* on_write_attributes )( Object, XmlWriter );
	void 			(* on_write_data )		( Object, XmlWriter );
	void 			(* on_write_end )		( Object, XmlWriter );

	/* XmlParser callback methods */
	XmlReadState	(* on_read_beginElement)( Object, XmlParser, XmlReadState, const char * name, XmlAttribs );
	void			(* on_read_endElement )	( Object, XmlParser, XmlReadState );
	void			(* on_read_text )		( Object, XmlParser, XmlReadState, const char * text );
	void			(* on_read_child )		( Object, XmlParser, XmlReadState, Object child );
	void			(* on_read_comment )	( Object, XmlParser, XmlReadState, const char * comment );

EndOfInterface;

/* Xml class registration
 */

struct _XmlClassRegisterItem
{
	const Class						class;
	int								context;
	ROM char *						name;
	struct _XmlClassRegisterItem *	next;
};

void _xml_register_class( struct _XmlClassRegisterItem * );

#define xml_register_class( pClass, pContext, pName )			\
do { 															\
	static struct _XmlClassRegisterItem xmlClass;				\
	xmlClass.class = & pClass ## Class;							\
	xmlClass.context = pContext;								\
	xmlClass.name = pName;										\
	xmlClass.next = NULL; 										\
	_xml_register_class( &xmlClass ); 							\
} while(0)

/* XmlException
*/

enum XmlErrorCodes
{
	XML_ERROR_NONE = 0,
	XML_ERROR_BUFFER_FULL,
	XML_ERROR_DIRECTION,
	XML_ERROR_FILE_ASSIGNMENT,
	XML_ERROR_PARAMETER,
	XML_ERROR_SEQUENCE,
	XML_ERROR_IO,
	XML_ERROR_SYNTAX,
	XML_ERROR_BAD_NESTING,

	XML_ERROR_NO_ATTRIBS,
	XML_ERROR_IMPLEMENTATION,
};

DeclareClass( XmlException, Exception );

/* usefull tools
 */

# define ooc_strdup( x ) ( (x != NULL) ? strdup(x) : (x) )

#endif  /* OOC_XML_H */
