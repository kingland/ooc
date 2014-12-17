
/* This is a XmlWriter class implementation file
 */

#include <stdarg.h>
#include <string.h>

#include "xml.h"
#include "implement/xml.h"

#include <ooc/list.h>

/** @class XmlWriter
 *  @brief XmlWriter class - brief description.
 * 	@see xmlwriter.h
 */

/** @file xmlwriter.h 
 *  @brief XmlWriter class - brief description.
 * XmlWriter is a .....
 * @note	This class is a final class, can not be inherited.
 */ 

/* Declaring the members of the class. 
 * They are all private.
 */

enum XmlWriterState
{
	STATE_NOTHING = 0,
	STATE_BOM,
	STATE_VERSION,
	STATE_PROLOG,
	STATE_BEGIN_ELEMENT,
	STATE_ATTRIBUTES,
	STATE_TEXT,
	STATE_TEXT_NOINDENT,
	STATE_END_ELEMENT,
	STATE_BEGIN_COMMENT,
	STATE_COMMENT_TEXT,
	STATE_COMMENT_NOINDENT,
	STATE_END_COMMENT
};

ClassMembers( XmlWriter, XmlBase )

	FILE *						file;
	char *						buffer;
	size_t						buffer_length;

	enum XmlWriterState			state;
	List						elements;

	int							indent;
	int							indent_position;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

Virtuals( XmlWriter, XmlBase )
EndOfVirtuals;

AllocateClass( XmlWriter, XmlBase );

/* Class initializing
 */

static
void
XmlWriter_initialize( Class this )
{
	ooc_init_class( List );
	ooc_init_class( XmlException );
}

/* Class finalizing
 */

static
void
XmlWriter_finalize( Class this )
{
	/* Release global resources! */
}


/* Constructor
 */

static
void
XmlWriter_constructor( XmlWriter self, const void * param )
{
	assert( ooc_isInitialized( XmlWriter ) );
	
	chain_constructor( XmlWriter, self, NULL );

	self->elements 	= list_new( NULL );
}

/* Destructor
 */

static
void
XmlWriter_destructor( XmlWriter self, XmlWriterVtable vtab )
{
	ooc_delete_and_null( (Object*) & self->elements );
}

/* Copy constuctor
 */

static
int
XmlWriter_copy( XmlWriter self, const XmlWriter from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*	=====================================================
	Class member functions
 */


XmlWriter
xmlwriter_new_file( FILE * file )
{
	XmlWriter self;

	ooc_init_class( XmlWriter );
		
	if( file == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_FILE_ASSIGNMENT ) );

	self =  ooc_new( XmlWriter, NULL );
	self->file = file;
	return self;
}

void
xml_set_indent( XmlWriter self, int indent )
{
	ooc_cast( self, XmlWriter );

	if( indent < -1 || indent > 16 )
		ooc_throw( xmlexception_new( XML_ERROR_PARAMETER ) );

	self->indent = indent;
}

int
xml_get_indent( XmlWriter self )
{
	ooc_cast( self, XmlWriter );

	return self->indent;
}

typedef
struct
EscapeBuffer
{
	char * 		converted;
	int			must_free;
} * EscapeBuffer;

static
void
escape( EscapeBuffer buffer, const char * text )
{
	size_t text_len, escaped_len;
	const char * tp;
	char * ep;

	for( text_len = escaped_len = 0, tp = text; *tp; tp++, text_len++ )
		{
			/* TODO: handling UTF-8 sequences here */
			switch( *tp ) {
				case '<' :		escaped_len += sizeof( ESCAPED_LT )-1; break;
				case '>' :		escaped_len += sizeof( ESCAPED_GT )-1; break;
				case '&' :		escaped_len += sizeof( ESCAPED_AMP )-1; break;
				case '\'' :		escaped_len += sizeof( ESCAPED_APOS )-1; break;
				case '\"' :		escaped_len += sizeof( ESCAPED_QUOT )-1; break;
				default :		escaped_len++;
			}
		}
	if( text_len == escaped_len ) {
		buffer->converted = (char *) text;
		buffer->must_free = FALSE;
		}
	else {
		buffer->converted = ooc_malloc( escaped_len + 1 );
		buffer->must_free = TRUE;
		for( tp = text, ep = buffer->converted; *tp; tp++ )
			{
				/* TODO: handling UTF-8 sequences here */
				switch( *tp ) {
					case '<' :		strcpy( ep, ESCAPED_LT );
									ep += sizeof( ESCAPED_LT )-1;
									break;
					case '>' :		strcpy( ep, ESCAPED_GT );
									ep += sizeof( ESCAPED_GT )-1;
									break;
					case '&' :		strcpy( ep, ESCAPED_AMP );
									ep += sizeof( ESCAPED_AMP )-1;
									break;
					case '\'' :		strcpy( ep, ESCAPED_APOS );
									ep += sizeof( ESCAPED_APOS )-1;
									break;
					case '\"' :		strcpy( ep, ESCAPED_QUOT );
									ep += sizeof( ESCAPED_QUOT )-1;
									break;
					default :		*ep++ = *tp;
				}
			}
		*ep = '\0';
		}
}

static
void
escapebuffer_release( EscapeBuffer buffer )
{
	if( buffer->must_free == TRUE )
		ooc_free( buffer->converted );
}

static
void
xml_write_formatted( XmlWriter self, const char * format, ... )
{
	va_list 	args;
	int			ret_val;

	if( self->file == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_FILE_ASSIGNMENT ) );

	va_start( args, format );
	ret_val = vfprintf( self->file, format, args );
	va_end (args);

	/* TODO: passing error information into the XmlException object */
	if( ret_val < 0 )
		ooc_throw( xmlexception_new( XML_ERROR_IO ) );
}

static
void
xml_write_nl( XmlWriter self )
{
	if( self->indent >= 0 )
	{
		if( self->indent_position < 0 )
			self->indent_position = 0;

		if( self->indent_position )
			xml_write_formatted( self, "\n%*s", self->indent_position, "" );
		else
			xml_write_formatted( self, "\n" );
	}
}

static
void
xml_indent_inc( XmlWriter self )
{
	if( self->indent > 0 )
	{
		self->indent_position += self->indent;
	}
}

static
void
xml_indent_dec( XmlWriter self )
{
	if( self->indent > 0 )
	{
		self->indent_position -= self->indent;
	}
}

static
void
xml_write_prolog( XmlWriter self )
{
	/* TODO: generating a proper XML prolog:
	 * BOM, version, encoding, doctype ... */

	xml_write_formatted(self, "%c%c%c", 0xEF, 0xBB, 0xBF ); /* Byte Order mark */

	xml_write_formatted( self, "<?xml version='1.0' encoding='UTF-8' ?>");

	self->state = STATE_PROLOG;
}

void
xml_write_begin_element( XmlWriter self, const char * name )
{
	ooc_cast( self, XmlWriter );

	if( self->state < STATE_PROLOG )
		xml_write_prolog( self );

	if( name == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_PARAMETER ) );

	switch( self->state )
	{
		case STATE_BEGIN_ELEMENT :
		case STATE_ATTRIBUTES :
										xml_write_formatted( self, ">");
										xml_indent_inc( self );
		case STATE_TEXT :
		case STATE_END_COMMENT :
		case STATE_END_ELEMENT :
		case STATE_PROLOG :				xml_write_nl( self );
										xml_write_formatted( self, "<%s", name );
										break;
		default :
			ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );
	}
	list_append( self->elements, (char*) name );
	self->state = STATE_BEGIN_ELEMENT;
}

void
xml_write_end_element( XmlWriter self )
{
	const char * name;

	ooc_cast( self, XmlWriter );

	name = (char *) list_remove_last_item( self->elements );

	if( name == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );

	switch( self->state )
	{
		case STATE_BEGIN_ELEMENT :
		case STATE_ATTRIBUTES :			xml_write_formatted( self, "/>" );
										break;
		case STATE_TEXT :
		case STATE_END_COMMENT :
		case STATE_END_ELEMENT :		xml_indent_dec( self );
										xml_write_nl( self );
		case STATE_TEXT_NOINDENT :		xml_write_formatted( self, "</%s>", name );
										break;
		default :
			ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );
	}
	self->state = STATE_END_ELEMENT;
}

void
xml_write_attribute( XmlWriter self, const char * name, const char * value )
{
	struct EscapeBuffer esc = { NULL, FALSE };

	ooc_manage( &esc, (ooc_destroyer) escapebuffer_release );

	ooc_cast( self, XmlWriter );

	/* TODO: handling the NULL value attribute. (could be necessary for HTML :-( ) */

	if( name == NULL || value == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_PARAMETER ) );

	switch( self->state )
	{
		case STATE_BEGIN_ELEMENT :
		case STATE_ATTRIBUTES :			escape( &esc, value );
										xml_write_formatted( self, " %s=\"%s\"", name, esc.converted );
										break;
		default :
			ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );
	}
	self->state = STATE_ATTRIBUTES;

	escapebuffer_release( ooc_pass( &esc ) );
}

void
xml_write_text( XmlWriter self, const char * text )
{
	enum XmlWriterState next_state = STATE_TEXT;

	struct EscapeBuffer esc = { NULL, FALSE };

	ooc_manage( &esc, (ooc_destroyer) escapebuffer_release );

	ooc_cast( self, XmlWriter );

	/* TODO: handling the NULL text element. (could be necessary for HTML :-( ) */

	if( text == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_PARAMETER ) );

	switch( self->state )
	{
		case STATE_BEGIN_ELEMENT :
		case STATE_ATTRIBUTES :			xml_write_formatted( self, ">" );
										xml_indent_inc( self );
		case STATE_END_COMMENT :
		case STATE_END_ELEMENT :		xml_write_nl( self );
		case STATE_TEXT :				escape( &esc, text );
										xml_write_formatted( self, "%s", esc.converted );
										break;

		case STATE_TEXT_NOINDENT :		escape( &esc, text );
										xml_write_formatted( self, ">%s", esc.converted );
										break;

		case STATE_BEGIN_COMMENT :		xml_indent_inc( self );
		case STATE_COMMENT_TEXT :		xml_write_nl( self );
		case STATE_COMMENT_NOINDENT :	xml_write_formatted( self, "%s", text );
										next_state = STATE_COMMENT_TEXT;
										break;
		default :
			ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );
	}
	self->state = next_state;

	escapebuffer_release( ooc_pass( &esc ) );
}

void
xml_write_element_text( XmlWriter self, const char * name, const char * text )
{
	xml_write_begin_element( self, name );
	self->state = STATE_TEXT_NOINDENT;
	xml_write_text( self, text );
	self->state = STATE_TEXT_NOINDENT;
	xml_write_end_element( self );
}

void
xml_write_begin_comment( XmlWriter self )
{
	ooc_cast( self, XmlWriter );

	if( self->state < STATE_PROLOG )
		xml_write_prolog( self );

	switch( self->state )
	{
		case STATE_BEGIN_ELEMENT :
		case STATE_ATTRIBUTES :
										xml_write_formatted( self, ">");
										xml_indent_inc( self );
		case STATE_TEXT :
		case STATE_END_ELEMENT :
		case STATE_END_COMMENT :
		case STATE_PROLOG :				xml_write_nl( self );
										xml_write_formatted( self, "<!-- " );
										break;
		default :
			ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );
	}
	self->state = STATE_BEGIN_COMMENT;
}

void
xml_write_end_comment( XmlWriter self )
{
	ooc_cast( self, XmlWriter );

	switch( self->state )
	{
		case STATE_BEGIN_COMMENT :
		case STATE_COMMENT_NOINDENT :	xml_write_formatted( self, " -->" );
										break;

		case STATE_COMMENT_TEXT :		xml_indent_dec( self );
										xml_write_nl( self );
										xml_write_formatted( self, "-->" );
										break;
		default :
			ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );
	}
	self->state = STATE_END_COMMENT;
}

void
xml_write_comment( XmlWriter self, const char * comment )
{
	ooc_cast( self, XmlWriter );

	xml_write_begin_comment( self );
	self->state = STATE_COMMENT_NOINDENT;
	xml_write_text( self, comment );
	self->state = STATE_COMMENT_NOINDENT;
	xml_write_end_comment( self );
}

