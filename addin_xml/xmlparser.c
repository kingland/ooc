
/* This is a XmlParser class implementation file
 *
 * Extensible Markup Language (XML) 1.1 (Second Edition)
 * W3C Recommendation 16 August 2006, edited in place 29 September 2006
 * http://www.w3.org/TR/2006/REC-xml11-20060816/
 *
 * EBNF visualizer:
 * http://railroad.my28msec.com/rr/ui
 *
 */


#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include "xml.h"
#include "implement/xml.h"
#include "xmlnode.h"

#define VALUE_STACK_SIZE 64
static void vs_clean( XmlParser self );

typedef
enum _Symbol
{
	/* Recognized symbols.
	 * These symbols can be recognized in the stream.
	 */
	end,
	space = ' ',
	quot = '"',
	apos = '\'',
	eqSign = '=',
	tagStart, tagStop,
	tagStartClose, tagEmptyClose,
	commentStart, commentStop,
	xmlDeclStart, xmlDeclStop,

	/* unrecognized symbols */
	characters = 0x100,	/* unrecognized symbols, can be any kind of character stream */

	/* Unrecognized symbols that can be checked against a rule.
	 * These symbols do NOT place a value onto the value stack.
	 */

	/* Unrecognized symbols that can be checked against a rule.
	 * These symbols place a value onto the value stack.
	 */
	bom,
	xmlDeclXML,
	name,
	escapedLiteral,
	comment,
	piLiteral,

	/* Symbols that resulted from a reduction.
	 * These symbols have a corresponding value on the value stack
	 * and may have status variables as well.
	 */
	tagName,	/*   */
	tagObject,	/*   */
	xmlAttribs, /*   */

} Symbol;

/* Processing instruction symbols look like XML declaration symbols */
#define	xmlPIStart xmlDeclStart
#define xmlPIStop xmlDeclStop


typedef int boolean;
#define false 0
#define true (!false)

typedef
struct SymbolValueItem
{
	char *			location;
	int				length;
}
SymbolValue;

typedef
struct ObjectValueItem
{
	Object			object;
	Xml				xml;
	const char *	xmlName;
}
ObjectValue;

typedef
struct ValueStackItem
{
	Symbol			symbol;
	XmlReadState	state;

	union
	{
	SymbolValue		symbolValue;
	ObjectValue		xmlObject;
	XmlAttribs		attribs;
	} v;
}
Value;

ClassMembers( XmlParser, XmlBase )

	char *				buffer;
	char *				next_pt;
	Symbol				sym;
	Symbol				string_delimiter;
	Value *				vs;
	int					vsp;
	int					vs_topmost_object;
	Class				object_type;

EndOfClassMembers;

Virtuals( XmlParser, XmlBase )
EndOfVirtuals;

AllocateClass( XmlParser, XmlBase );

static
void
XmlParser_initialize( Class this )
{
	ooc_init_class( XmlAttribs );
}

static
void
XmlParser_finalize( Class this )
{
}

static
void
XmlParser_constructor( XmlParser self, const void * params )
{
	assert( ooc_isInitialized( XmlParser ) );
	
	chain_constructor( XmlParser, self, NULL );

	self->vs = ooc_malloc( VALUE_STACK_SIZE * sizeof(Value) );
	self->vsp = -1;
	self->vs_topmost_object = self->vsp;
}

static
void
XmlParser_destructor( XmlParser self, XmlParserVtable vtab )
{
	ooc_free_and_null( (void**) & self->vs );
	ooc_free_and_null( (void**) & self->buffer );
}

static
int
XmlParser_copy( XmlParser self, const XmlParser from )
{
	return OOC_NO_COPY;
}

/*	=====================================================
	Class member functions
 */


XmlParser
xmlparser_new_str( char * str )
{
	XmlParser self;

	ooc_init_class( XmlParser );
		
	self = ooc_new( XmlParser, NULL );

	self->buffer = str;

	self->next_pt = self->buffer;

	return self;
}

void
_xmlparser_set_type( XmlParser self, Class type )
{
	assert( ooc_isInstanceOf( self, XmlParser ) );
	assert( _ooc_isInitialized( type ) );

	self->object_type = type;
}

void
_xmlparser_set_root( XmlParser self, Object rootObject )
{
	ooc_manage_object( rootObject );

	assert( self->vsp == -1 );

	self->vs[0].symbol = tagObject;
	self->vs[0].v.xmlObject.object = rootObject;
	self->vs[0].v.xmlObject.xmlName = NULL;
	self->vs[0].v.xmlObject.xml = ooc_get_interface_must_have( rootObject, Xml );
	self->vs[0].state = 0;

	self->vsp = 0;
	self->vs_topmost_object = self->vsp;

	ooc_pass( rootObject );
}

static
struct _EscapeTab
{
	const char * 	escaped;
	const char *	unescaped;
	size_t			len;
} EscapeTab[] =
				{
					{ ESCAPED_LT, "<", sizeof(ESCAPED_LT) },
					{ ESCAPED_GT, ">", sizeof(ESCAPED_GT) },
					{ ESCAPED_AMP, "&", sizeof(ESCAPED_AMP) },
					{ ESCAPED_APOS, "'", sizeof(ESCAPED_APOS) },
					{ ESCAPED_QUOT, "\"", sizeof(ESCAPED_QUOT) },
				};

#define ESCAPE_TAB_ITEMS (sizeof(EscapeTab)/sizeof(struct _EscapeTab))

static
const char *
isEscaped( char ** rp )
{
	int i;

	for( i=0; i<ESCAPE_TAB_ITEMS; i++)
		if( strncmp( *rp, EscapeTab[i].escaped, EscapeTab[i].len ) == 0 )
		{
			(*rp) += EscapeTab[i].len - 1;
			return EscapeTab[i].unescaped;
		}
	return NULL;
}

static
char *
unescape( char * const orig )
{
	char * r = orig;
	char * w;
	const char * unescaped;

	if( r )
	{
		while( *r && *r != '&' )
			r++;
		if( *r ) {
			w = r;
			while( *r )
			{
				if( *r == '&' && ( unescaped = isEscaped( &r ) ) )
					while( *unescaped )
						*w++ = *unescaped++;
				else
					*w++ = *r++;
			}
			*w = *r; /* copy the tailing zero */
		}
	}
	return orig;
}

#define p 		(self->next_pt)
#define	vs		(self->vs)
#define vsp		(self->vsp)
#define sym		(self->sym)

static
void
vs_push_symbol( XmlParser self, Symbol s, char * pos, int len )
{
	assert( vsp < VALUE_STACK_SIZE - 1 );
	++vsp;
	vs[ vsp ].symbol = s;
	vs[ vsp ].v.symbolValue.location = pos;
	vs[ vsp ].v.symbolValue.length 	= len;

	p += len;
}

static
void
vs_push_attribs( XmlParser self, XmlAttribs attrs )
{
	assert( vsp < VALUE_STACK_SIZE - 1 );
	++vsp;
	vs[ vsp ].symbol = xmlAttribs;
	vs[ vsp ].v.attribs = attrs;
}

static
void
vs_push_object( XmlParser self, Object obj, const char * xmlName )
{
	assert( vsp < VALUE_STACK_SIZE - 1 );
	++vsp;
	vs[ vsp ].symbol = tagObject;
	vs[ vsp ].v.xmlObject.object = obj;
	vs[ vsp ].v.xmlObject.xmlName = xmlName;
	vs[ vsp ].v.xmlObject.xml = ooc_get_interface_must_have( obj, Xml );

	self->vs_topmost_object = vsp;
}

static
const char *
vs_get_symbol_value( XmlParser self, int offset )
{
	assert( offset >= -vsp && offset <= 0 );

	*( vs[vsp + offset ].v.symbolValue.location + vs[vsp + offset ].v.symbolValue.length ) = '\0';
	if( vs[vsp + offset ].symbol == escapedLiteral )
		return unescape( vs[ vsp + offset ].v.symbolValue.location );
	else
		return vs[ vsp + offset ].v.symbolValue.location;
}

int
vs_get_topmost_object(XmlParser self )
{
	int lastObject;

	assert( vsp > 0 );
	if( self->vs_topmost_object < 0 || self->vs_topmost_object >= vsp )
	{
		for( lastObject = vsp-1; lastObject >=0; lastObject-- )
			if( ( vs[ lastObject ].symbol = tagObject ) )
				break;
	}
	else
		lastObject = self->vs_topmost_object;

	assert( lastObject >= 0 );
	return lastObject;
}

static
void
vs_drop_top_n( XmlParser self, int n )
{
	assert( n >= 1 );
	assert( vsp >= n-1 );

	vsp -= n;
							/* Should Objects be deleted ?
							 * No! They are passed to other Objects or freed while reducing. */
}

#define vs_drop_top( self ) vs_drop_top_n( self, 1 )

static
void
vs_clean( XmlParser self )
{
	while( vsp >=0 )
	{
		switch( vs[vsp].symbol )
		{
			case tagObject	:	ooc_delete( (Object) vs[vsp].v.xmlObject.object );
								break;
			case xmlAttribs :	ooc_delete( (Object) vs[vsp].v.attribs );
								break;

			default:			break;
		}
		vs_drop_top( self );
	}
}

static
void
error( const char * msg, ... )
{
/*	TODO: Add the message to the exception object
 *
 	va_list 	args;

	va_start( args, msg );
	vprintf( msg, args );
	va_end (args);
*/
	ooc_throw( xmlexception_new( XML_ERROR_SYNTAX ) );
}


/* The lexer
 */

static
boolean
check_name( XmlParser self )
{
	size_t len = 0;
	const char * c = p;

	/* name = ["a-zA-Z_"]["0-9:a-zA-Z_"]+ ;
	 * TODO: this is not good !!! the ':' must be handled correctly */

	if( isalpha( *c ) || *c == '_' )
	{
		len++; c++;
		while( isalnum( *c ) || *c == '_' || *c == ':' )
			len++, c++;
		vs_push_symbol( self, name, p, len );
		return true;
	}
	else
		return false;
}


static
size_t
calc_charReference_len( XmlParser self, const char * c )
{
	/* [66]   	CharRef	   ::=   	'&#' [0-9]+ ';'
									| '&#x' [0-9a-fA-F]+ ';'
	*/

	boolean hex;
	size_t len = 2;

	assert( *c == '&' && *(c+1) == '#' );

	c += len;
	if( *c == 'x' )
		hex = true, len++, c++;
	else
		hex = false;

	while( *c != '\0' && *c != ';' )
		if( (!hex && isdigit( *c )) || (hex && isxdigit( *c )) )
			len ++, c++;
		else
			return 0;

	if( len > 2 && *c == ';' )
		return len + 1;
	else
		return 0;
}

static
size_t
calc_name_len( XmlParser self, const char * c )
{
	/*
	[4]   	NameStartChar	   ::=   	":" | [A-Z] | "_" | [a-z] | [#xC0-#xD6] | [#xD8-#xF6] | [#xF8-#x2FF] | [#x370-#x37D] | [#x37F-#x1FFF] | [#x200C-#x200D] | [#x2070-#x218F] | [#x2C00-#x2FEF] | [#x3001-#xD7FF] | [#xF900-#xFDCF] | [#xFDF0-#xFFFD] | [#x10000-#xEFFFF]
	[4a]   	NameChar	   ::=   	NameStartChar | "-" | "." | [0-9] | #xB7 | [#x0300-#x036F] | [#x203F-#x2040]
	[5]   	Name	   ::=   	NameStartChar (NameChar)*
	*/

	size_t len = 1;

	assert( *c == '&' );

	c += len;

	while( *c != '\0' && *c != ';' )
		if( isalnum( *c ) ) /* TODO: This is just a quick and  dirty test! */
			len ++, c++;
		else
			return 0;

	if( len > 1 && *c == ';' )
		return len + 1;
	else
		return 0;


	return 0;
}

static
size_t
calc_reference_len( XmlParser self, const char * c )
{
	/* [67]   	Reference	   ::=   	 EntityRef | CharRef
	   [68]   	EntityRef	   ::=   	'&' Name ';'
	 */

	assert( *c == '&' );

	if( *(c+1) == '#' )
		return calc_charReference_len( self, c );
	else
		return calc_name_len( self, c );
}

static
boolean
check_escapedLiteral( XmlParser self )
{
	size_t len = 0;
	const char * c = p;

	/* escapedLiteral = [^<^>]* ; */
	/* CharData	   ::=   	[^<&]* - ([^<&]* ']]>' [^<&]*) */

	while( *c != self->string_delimiter && *c != '\0' && *c != '<' && *c != '>' )
		if( *c != '&' )
			len++, c++;
		else
		{
			size_t	reference_len = calc_reference_len( self, c );
			if( reference_len == 0 )
			{
				len = 0;
				break;
			}
			else
				len += reference_len, c += reference_len;
		};

	if( len )
		vs_push_symbol( self, escapedLiteral, p, len );

	return len != 0;
}

static
boolean
check_piLiteral( XmlParser self )
{
	size_t len = 0;
	const char * c = p;

	/* PILiteral = (Char* - (Char* '?>' Char*)))? ; */

	while( *c != '\0' && !( *c == '?' && *(c+1) == '>' ) )
		len++, c++;

	if( len )
		vs_push_symbol( self, piLiteral, p, len );

	return len != 0;
}

static
boolean
check_xmlDeclXML( XmlParser self )
{
	if( toupper(*p) == 'X' && toupper(*(p+1)) == 'M' && toupper(*(p+2)) == 'L' ) {
		vs_push_symbol( self, xmlDeclXML, p, 3 );
		return true;
	}
	return false;
}

static
boolean
check_comment( XmlParser self )
{
	char * c = p;
	size_t	len = 0;

	while( *c != '\0' && ( *c != '-' || *(c+1) != '-' ) )
		c++, len++;

	while( c != p && ( *(c-1) == ' ' || *(c-1) == '\t' || *(c-1) == '\n' || *(c-1) == '\r' ) )
		c--, len--;

	vs_push_symbol( self, comment, p, len );

	return true;
}

static
void
getsym( XmlParser self )
{
	switch( *p++ ) {
	case '<' :	if		( *p == '?' ) 	{ p++; sym = xmlDeclStart; break; }
				else if ( *p == '/' ) 	{ p++; sym = tagStartClose; break; }
				else if ( *p == '!' && *(p+1) == '-' && *(p+2) == '-' )
										{ p+=3; sym = commentStart; break; }
				else					{      sym = tagStart; break; };
	case '/' :	if		( *p == '>' )	{ p++; sym = tagEmptyClose; break; };
				goto unrecognized;
	case '?' :	if		( *p == '>' )	{ p++; sym = xmlDeclStop; break; };
				goto unrecognized;
	case '>' :  sym = tagStop; break;
	case '-' :	if ( *p == '-' && *(p+1) == '>' )
										{ p+=2; sym = commentStop; break; };
				goto unrecognized;
	case '"' :	sym = quot; break;
	case '\'':	sym = apos; break;
	case '=' :  sym = eqSign; break;
	case ' ' :
	case '\t':
	case '\r':
	case '\n':	while( *p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' )
					p++;
				sym = space; break;
	case '\0':	p--; sym = end; break;
	default	 :
	unrecognized: p--; sym = characters; break;
	}
}

static
boolean
checksym( XmlParser self, Symbol s )
{
	boolean retval = false;

	if( sym == characters || s == escapedLiteral )
	{
		switch( s ) {
			case name :				retval = check_name( self );
									break;
			case escapedLiteral : 	retval = check_escapedLiteral( self );
									break;
			case xmlDeclXML :		retval = check_xmlDeclXML( self );
									break;
			case piLiteral : 		retval = check_piLiteral( self );
									break;
			case comment :			retval = check_comment( self );
									break;
			default :
				break;
		}
	}
	return retval;
}


/* The parser
 */

static
boolean
accept( XmlParser self, Symbol s )
{
	boolean retval;

	if( ( sym == characters &&  s > characters ) || s == escapedLiteral  )
		retval = checksym( self, s );
	else
		retval = ( sym == s );

	if( retval )
		getsym( self );

	return retval;
}

static
void
expect( XmlParser self, Symbol s )
{
	if( ! accept( self, s ) )
		error( "symbol %d expected\n", s );
}

static
void
expectString( XmlParser self )
{
	if( ! accept( self, self->string_delimiter = quot ) )
		expect( self, self->string_delimiter = apos );
	expect( self, escapedLiteral );
	expect( self, self->string_delimiter );
}

static
int
acceptText( XmlParser self )
{
	self->string_delimiter = '\0';
	return accept( self, escapedLiteral );
}

static
void
reduceTagName( XmlParser self )
{
	/* The element tag name is on the top is the stack.
	 * The object must be instantiated here.
	 */
	assert( vs[vsp].symbol == name );

	if( self->object_type )
	{
		const char * name;

		name = vs_get_symbol_value( self, 0 );
		vs_drop_top( self ); /* name */
		vs_push_object( self, ooc_new_classptr( self->object_type, NULL ), name );
	}
	else if( /* can be instantiated */ 0 )
	{

	}
	else
		vs[vsp].symbol = tagName;
}

static
void
reduceAttrib( XmlParser self )
{
	assert( vsp >= 2 );
	assert( vs[vsp].symbol == escapedLiteral );
	assert( vs[vsp-1].symbol == name );
	assert( vs[vsp-2].symbol == xmlDeclXML || vs[vsp-2].symbol == tagObject || vs[vsp-2].symbol == tagName || vs[vsp-2].symbol == xmlAttribs );

	if( vs[ vsp-2 ].symbol == xmlAttribs )
	{
		xmlattribs_append( vs[ vsp-2 ].v.attribs, vs_get_symbol_value( self, -1 ), vs_get_symbol_value( self, 0 ) );
		vs_drop_top_n( self, 2 );
	}
	else
	{
		XmlAttribs attrs = xmlattribs_new();
		ooc_manage_object( attrs );
		xmlattribs_append( attrs, vs_get_symbol_value( self, -1 ), vs_get_symbol_value( self, 0 ) );
		vs_drop_top_n( self, 2 );
		vs_push_attribs( self, ooc_pass( attrs ) );
	}
}

static
void
reduceBeginElement( XmlParser self )
{
	assert( vsp >= 1 );
	assert( vs[vsp].symbol == xmlAttribs && ( vs[vsp-1].symbol == tagName || vs[vsp-1].symbol == tagObject ) );

	if( vs[vsp-1].symbol == tagObject )
	{
		if( vs[vsp-1].v.xmlObject.xml->on_read_beginElement )
			vs[vsp-1].state = \
			vs[vsp-1].v.xmlObject.xml->on_read_beginElement( vs[vsp-1].v.xmlObject.object,
															  self, -1,
															  vs[vsp-1].v.xmlObject.xmlName,
															  vs[vsp].v.attribs );
	}
	else /* tagName */
	{
		int topmostObject = vs_get_topmost_object( self );
		if( vs[topmostObject].v.xmlObject.xml->on_read_beginElement )
			vs[vsp-1].state = \
			vs[topmostObject].v.xmlObject.xml->on_read_beginElement( vs[topmostObject].v.xmlObject.object,
																	 self, vs[vsp-2].state,
																	 vs_get_symbol_value( self, -1 ),
																	 vs[vsp].v.attribs );
	}
	ooc_delete( (Object) vs[vsp].v.attribs );
	vs_drop_top( self ); /* xmlAttribs */
}

static
void
reduceText( XmlParser self )
{
	assert( vsp >= 1 );
	assert( vs[vsp].symbol == escapedLiteral );
	assert( vs[vsp-1].symbol == tagObject || vs[vsp-1].symbol == tagName );

	if( vs[vsp-1].symbol == tagObject ) {
		if( vs[vsp-1].v.xmlObject.xml->on_read_text )
			vs[vsp-1].v.xmlObject.xml->on_read_text( vs[vsp-1].v.xmlObject.object, self, vs[vsp-1].state, vs_get_symbol_value( self, 0 ) );
	}
	else if(  vs[vsp-1].symbol == tagName )
	{
		int lastObject = vs_get_topmost_object( self );
		if( vs[lastObject].v.xmlObject.xml->on_read_text )
			vs[lastObject].v.xmlObject.xml->on_read_text( vs[lastObject].v.xmlObject.object, self, vs[vsp-1].state, vs_get_symbol_value( self, 0 ) );
	}
	else
		ooc_throw( xmlexception_new( XML_ERROR_IMPLEMENTATION ) );

	vs_drop_top( self ); /* escapedLiteral */
}

static
void
reduceComment( XmlParser self )
{
	assert( vsp >= 1 );
	assert( vs[vsp].symbol == comment );
	assert( vs[vsp-1].symbol == tagObject || vs[vsp-1].symbol == tagName );

	if( vs[vsp-1].symbol == tagObject ) {
		if( vs[vsp-1].v.xmlObject.xml->on_read_comment )
			vs[vsp-1].v.xmlObject.xml->on_read_comment( vs[vsp-1].v.xmlObject.object, self, vs[vsp-1].state, vs_get_symbol_value( self, 0 ) );
	}
	else if(  vs[vsp-1].symbol == tagName )
	{
		int lastObject = vs_get_topmost_object( self );
		if( vs[lastObject].v.xmlObject.xml->on_read_comment )
			vs[lastObject].v.xmlObject.xml->on_read_comment( vs[lastObject].v.xmlObject.object, self, vs[vsp-1].state, vs_get_symbol_value( self, 0 ) );
	}
	else
		ooc_throw( xmlexception_new( XML_ERROR_IMPLEMENTATION ) );

	vs_drop_top( self ); /* escapedLiteral */
}


static
void
reduceEmptyEndElement( XmlParser self )
{
	assert( vsp >= 0 );
	assert( vs[vsp].symbol == tagObject || vs[vsp].symbol == tagName );

	if( vs[vsp].symbol == tagObject )
	{
		if( vs[vsp].v.xmlObject.xml->on_read_endElement )
			vs[vsp].v.xmlObject.xml->on_read_endElement( vs[vsp].v.xmlObject.object, self, vs[vsp].state );
		if( vsp >= 1 )
		{
			int topmostObject = vs_get_topmost_object( self );
			if( vs[topmostObject].v.xmlObject.xml->on_read_child )
				vs[topmostObject].v.xmlObject.xml->on_read_child( vs[topmostObject].v.xmlObject.object, self, vs[topmostObject].state, vs[vsp].v.xmlObject.object );
		}
	}
	else /* tagName */
	{
		assert( vsp >= 1 );
		int topmostObject = vs_get_topmost_object( self );
		if( vs[topmostObject].v.xmlObject.xml->on_read_endElement )
			vs[topmostObject].v.xmlObject.xml->on_read_endElement( vs[topmostObject].v.xmlObject.object, self, vs[vsp].state );
	}
	if( vsp > 0)
		vs_drop_top( self );
}

static
void
reduceEndElement( XmlParser self )
{
	assert( vsp >= 1 );
	assert( vs[vsp].symbol == name );
	assert( vs[vsp-1].symbol == tagObject || vs[vsp-1].symbol == tagName );

	if( vs[vsp-1].symbol == tagObject )
	{
		if( strcmp( vs[vsp-1].v.xmlObject.xmlName, vs_get_symbol_value( self, 0 ) ) != 0 )
			ooc_throw( xmlexception_new( XML_ERROR_BAD_NESTING ) );
	}
	else /* tagName */
	{
		if( strcmp( vs_get_symbol_value( self, -1 ), vs_get_symbol_value( self, 0 ) ) != 0 )
			ooc_throw( xmlexception_new( XML_ERROR_BAD_NESTING ) );
	}
	vs_drop_top( self ); /* name */

	reduceEmptyEndElement( self );
}

static
void
reduceProcessingInstruction( XmlParser self )
{
	assert( vs[vsp].symbol == name
			|| ( vs[vsp-1].symbol == name && vs[vsp].symbol == piLiteral ) );

	/* TODO: PI reduction here */
	/* the name can not be XML case insensitive ! */

	if( vs[vsp].symbol == name )
		vs_drop_top( self );
	else
		vs_drop_top_n( self, 2 );
}

static
int
acceptPI( XmlParser self )
{
	int retval = false;

	if( accept( self, xmlPIStart ) )
	{
		expect( self, name );
		accept( self, space );
		accept( self, piLiteral );
		expect( self, xmlPIStop );
		retval = true;
	}
	return retval;
}

static
void
acceptAttribList( XmlParser self )
{
	accept( self, space );
	while( accept( self, name ) )
	{
									accept( self, space );
		expect( self, eqSign ); 	accept( self, space );
		expectString( self ); 		accept( self, space );
		reduceAttrib( self );
	}
	if( vs[vsp].symbol != xmlAttribs )
		vs_push_attribs( self, xmlattribs_new() );
}

static
void
acceptXmlDecl( XmlParser self )
{
	if( accept( self, xmlDeclStart ) ) {
		expect( self, xmlDeclXML );
		acceptAttribList( self );
		expect( self, xmlDeclStop );

		/* TODO: reduction here */
		assert( vs[vsp-1].symbol == xmlDeclXML && vs[vsp].symbol == xmlAttribs );
		ooc_delete( (Object) vs[vsp].v.attribs );
		vs_drop_top_n( self, 2 );
	}
}

static
void
acceptProlog( XmlParser self )
{
	 accept( self, space );
	 acceptXmlDecl( self );
}

static
void
xmlElement( XmlParser self )
{
	/* TODO: bad logic, accepts any text before a tag */
	while( true ) {
		accept( self, space );
		if( accept( self, tagStart ) ) {
									accept( self, space );
			expect( self, name );
			reduceTagName( self );
			acceptAttribList( self );
			reduceBeginElement( self );
			if( accept( self, tagEmptyClose ) )
				reduceEmptyEndElement( self );
			else {
				expect( self, tagStop );
			}
		}
		else if( accept( self, tagStartClose )) {
									accept( self, space );
			expect( self, name ); 	accept( self, space );
			expect( self, tagStop );
			reduceEndElement( self );
		}
		else if( accept( self, commentStart ) ) {
			accept( self, space );
			expect( self, comment );
			accept( self, space );
			expect( self, commentStop );
			reduceComment( self );
		}
		else if( acceptPI( self ) ) {
			reduceProcessingInstruction( self );
		}
		else if( acceptText( self ) ) {
			reduceText( self );
		}
		else
			break;
	}
}

static
void
xmlFile( XmlParser self )
{
	getsym( self );
	accept( self, bom );
	acceptProlog( self );
	xmlElement( self);
	expect( self, end );
}

Object
xmlparser_parse( XmlParser self )
{
	try
	{
		xmlFile( self );
	}
	catch_any
	{
		vs_clean( self );
		ooc_rethrow();
	}
	end_try;

	assert( vsp == 0 );
	assert( vs[0].symbol == tagObject );

	return vs[0].v.xmlObject.object;
}
