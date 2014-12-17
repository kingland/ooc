
/* This is a XmlParserTest class implementation file
 */

#include <ooc/testcase.h>

#include "../xmlparser.h"
#include "../xmlwriter.h"
#include "../xmlnode.h"

#include <string.h>

#ifdef OOC_NO_FINALIZE
#define ooc_finalize_class( x )
#define ooc_finalize_all( )
#endif


/** @class XmlParserTest
 *  @brief XmlParserTest class - brief description.
 * 	@see xmlparsertest.h
 */

/** @file xmlparsertest.h 
 *  @brief XmlParserTest class - brief description.
 * XmlParserTest is a .....
 * @note	This class is a final class, can not be inherited.
 * @note	Run as: valgrind --leak-check=yes --quiet ./xmlparsertest
 */ 

DeclareClass( XmlParserTest, TestCase );

Virtuals( XmlParserTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( XmlParserTest, TestCase )

	int		data;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( XmlParserTest, TestCase );


/* Class initializing
 */

static void xmlparsertest_before_class( XmlParserTest self );
static void xmlparsertest_before( XmlParserTest self );
static void xmlparsertest_after( XmlParserTest self );
static void xmlparsertest_after_class( XmlParserTest self );

static
void
XmlParserTest_initialize( Class this )
{
	XmlParserTestVtable vtab = & XmlParserTestVtableInstance;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) xmlparsertest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) xmlparsertest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) xmlparsertest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) xmlparsertest_after_class;

	ooc_init_class( Exception );
	
	ooc_init_class( XmlParser );
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE

static
void
XmlParserTest_finalize( Class this )
{
	/* Release global resources! */
}

#endif

/* Constructor
 */

static
void
XmlParserTest_constructor( XmlParserTest self, const void * params )
{
	assert( ooc_isInitialized( XmlParserTest ) );
	
	chain_constructor( XmlParserTest, self, params );
}

/* Destructor
 */

static
void
XmlParserTest_destructor( XmlParserTest self, XmlParserTestVtable vtab )
{
}

/* Copy constuctor
 */

static
int
XmlParserTest_copy( XmlParserTest self, const XmlParserTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
xmlparsertest_before_class( XmlParserTest self )
{
	return;
}

static
void
xmlparsertest_before( XmlParserTest self )
{
	return;
}

static
void
xmlparsertest_after( XmlParserTest self )
{
	return;
}

static
void
xmlparsertest_after_class( XmlParserTest self )
{
	return;
}


/*	=====================================================
	Test methods
 */

void
static
xmlparsertest_method1( XmlParserTest self )
{
	XmlNode		xmlObject = NULL;
	XmlParser 	xmlp = xmlparser_new_str( strdup( "\t <?xml\tversion='1.0' encoding=\"UTF-8\" ?> <html><head/><body elements='1' scraps=\"szar&amp;\" >body ez ám a javából</body><!-- \n  \rSome comment here \t   --></html>   " ) );
	{
		ooc_manage_object( xmlp );

		xmlObject = xmlnode_parse( xmlp );
		{
			ooc_manage_object( xmlObject );
			XmlWriter xmlw = xmlwriter_new_file( stdout );
			{
				ooc_manage_object( xmlw );

				xmlnode_write( xmlObject, xmlw );

				ooc_delete( (Object) ooc_pass( xmlw ) );
			}
			ooc_delete( (Object) ooc_pass( xmlObject ) );
		}
		ooc_delete( (Object) ooc_pass( xmlp ) );
	}
}

/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
static ROM_ALLOC struct TestCaseMethod methods[] =
{
	
	TEST(xmlparsertest_method1),
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
TESTCASE_MAIN
{
	XmlParserTest xmlparsertest;
	int result;
	
	ooc_init_class( XmlParserTest );
	xmlparsertest = ooc_new( XmlParserTest, &methods );
	result = testcase_run((TestCase)xmlparsertest);
	ooc_delete( (Object) xmlparsertest );
	ooc_finalize_all();
	return result;
}
