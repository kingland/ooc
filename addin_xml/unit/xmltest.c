
/* This is a XmlTest class implementation file
 */

#include <ooc/testcase.h>

#include "../xml.h"

#include <ooc/implement/exception.h>


#ifdef OOC_NO_FINALIZE
#define ooc_finalize_class( x )
#define ooc_finalize_all( )
#endif

enum XmlTestExceptionCodes
{
	error_none = 0,
	error_file_cant_open,
	error_file_not_found,
	error_no_ref_file_given
};

DeclareClass( XmlTestException, Exception );

ClassMembers( XmlTestException, Exception )
EndOfClassMembers;

Virtuals( XmlTestException, Exception )
EndOfVirtuals;

AllocateClass( XmlTestException, Exception );

static	void	XmlTestException_initialize( Class this ) {}
static	void	XmlTestException_finalize( Class this ) {}

static	void	XmlTestException_constructor( XmlTestException self, const void * params )
{
	assert( ooc_isInitialized( XmlTestException ) );

	chain_constructor( XmlTestException, self, params );
}

static	void	XmlTestException_destructor( XmlTestException self, XmlTestExceptionVtable vtab ) {}
static	int		XmlTestException_copy( XmlTestException self, const XmlTestException from ) { return OOC_COPY_DEFAULT; }

static
Exception
xmltestexception_new( enum XmlTestExceptionCodes code )
{
	return (Exception) ooc_new( XmlTestException, & code );
}


/** @class XmlTest
 *  @brief XmlTest class - brief description.
 * 	@see xmltest.h
 */

/** @file xmltest.h 
 *  @brief XmlTest class - brief description.
 * XmlTest is a .....
 * @note	This class is a final class, can not be inherited.
 * @note	Run as: valgrind --leak-check=yes --quiet ./xmltest
 */ 

DeclareClass( XmlTest, TestCase );

Virtuals( XmlTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( XmlTest, TestCase )

	XmlWriter		xmlw;
	const char *	ref_file_name;
	FILE *			ref_file;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( XmlTest, TestCase );


/* Class initializing
 */

static void xmltest_before_class( XmlTest self );
static void xmltest_before( XmlTest self );
static void xmltest_after( XmlTest self );
static void xmltest_after_class( XmlTest self );

static
void
XmlTest_initialize( Class this )
{
	XmlTestVtable vtab = & XmlTestVtableInstance;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) xmltest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) xmltest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) xmltest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) xmltest_after_class;

	ooc_init_class( Exception );
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE

static
void
XmlTest_finalize( Class this )
{
	/* Release global resources! */
}

#endif

/* Constructor
 */

static
void
XmlTest_constructor( XmlTest self, const void * params )
{
	assert( ooc_isInitialized( XmlTest ) );
	
	chain_constructor( XmlTest, self, params );
}

/* Destructor
 */

static
void
XmlTest_destructor( XmlTest self, XmlTestVtable vtab )
{
}

/* Copy constuctor
 */

static
int
XmlTest_copy( XmlTest self, const XmlTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
xmltest_before_class( XmlTest self )
{
	return;
}

#define TMP_FILE_NAME "xmltest.tmp"

static
void
xmltest_before( XmlTest self )
{
	self->ref_file = fopen( TMP_FILE_NAME, "w+" );
	if( self->ref_file == NULL )
		ooc_throw( xmltestexception_new( error_file_cant_open ) );

	self->xmlw = xmlwriter_new_file( self->ref_file );
}

static
void
xmltest_after( XmlTest self )
{
	if( self->xmlw )
	{
		ooc_manage_object( self->xmlw );

		if( self->ref_file )
		{
			int comp_value;
			static char command[256]; /* bad, but it is ok here ;-) */

			fprintf( self->ref_file, "\n" );
			fclose( ooc_ptr_read_and_null( (void**) & self->ref_file ) );

			if( self->ref_file_name == NULL )
				ooc_throw( xmltestexception_new( error_no_ref_file_given ) );

			sprintf( command, "diff %s %s", self->ref_file_name, TMP_FILE_NAME );
			comp_value = system( command );

			assertZeroMsg( comp_value, "The generated file is not identical to the reference file" );
		}

		ooc_pass( self->xmlw );
		ooc_delete_and_null( (Object*) & self->xmlw );
	}
}

static
void
xmltest_after_class( XmlTest self )
{
	return;
}

/*	=====================================================
	Test methods
 */

static
void
xmltest_write_ref_1( XmlTest self )
{
	XmlWriter xmlw = self->xmlw;

	xml_write_begin_element( xmlw, "first" );

		xml_write_begin_element( xmlw, "head" );
		xml_write_comment( xmlw, "This is a comment in the head." );
		xml_write_begin_element( xmlw, "break" );
		xml_write_end_element( xmlw );
			xml_write_begin_element( xmlw, "script" );
			xml_write_attribute( xmlw, "type", "text/javascript" );
			xml_write_element_text( xmlw, "text", "this is an escaped text: <>&\"\'" );
			xml_write_text( xmlw, "this is a second text" );
			xml_write_end_element( xmlw );
			xml_write_element_text( xmlw, "epilog", "closing text" );
		xml_write_end_element( xmlw );
		xml_write_begin_element( xmlw, "body" );
			xml_write_attribute( xmlw, "owner", "Joe" );
			xml_write_attribute( xmlw, "date", "Today" );
			xml_write_attribute( xmlw, "escaped", "<>&\"\'" );
		xml_write_end_element( xmlw );
		xml_write_begin_element( xmlw, "body2" );
			xml_write_begin_comment( xmlw );
				xml_write_text( xmlw, "This is a longer comment. <non-escaped> & ampersand character" );
				xml_write_text( xmlw, "This is the second line of the longer comment with a \" quotation mark." );
			xml_write_end_comment( xmlw );
		xml_write_end_element( xmlw );

	xml_write_end_element( xmlw );
}

void
static
xmltest_method_1_1( XmlTest self )
{
	XmlWriter xmlw = self->xmlw;

	self->ref_file_name = "ref_1_1.xml";

	xml_set_indent( xmlw, -1 );

	xmltest_write_ref_1( self );
}

void
static
xmltest_method_1_2( XmlTest self )
{
	XmlWriter xmlw = self->xmlw;

	self->ref_file_name = "ref_1_2.xml";

	xmltest_write_ref_1( self );
}

void
static
xmltest_method_1_3( XmlTest self )
{
	XmlWriter xmlw = self->xmlw;

	self->ref_file_name = "ref_1_3.xml";

	xml_set_indent( xmlw, 2 );

	xmltest_write_ref_1( self );
}

/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
static ROM_ALLOC struct TestCaseMethod methods[] =
{
	
	TEST(xmltest_method_1_1),
	TEST(xmltest_method_1_2),
	TEST(xmltest_method_1_3),
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
TESTCASE_MAIN
{
	XmlTest xmltest;
	int result;
	
	ooc_init_class( XmlTest );
	xmltest = ooc_new( XmlTest, &methods );
	result = testcase_run((TestCase)xmltest);
	ooc_delete( (Object) xmltest );
	ooc_finalize_all();
	return result;
}

