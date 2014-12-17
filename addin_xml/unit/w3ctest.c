
/* This is a W3Ctest class implementation file
 */

#include <ooc/testcase.h>

#include "../xmlnode.h"

#ifdef OOC_NO_FINALIZE
#define ooc_finalize_class( x )
#define ooc_finalize_all( )
#endif


/** @class W3Ctest
 *  @brief W3Ctest class - a unit test class based on W3C test case recommendations
 * 	@see w3ctest.h
 */

/** @file w3ctest.h 
 *  @brief W3Ctest class - a unit test class based on W3C test case recommendations
 * W3Ctest is a test case that uses the W3C XML testsuite / test case recommendations as described at
 * http://www.w3.org/XML/Test/\n
 * The appropriate XML test suite must be downloaded.
 * @note	This class is a final class, can not be inherited.
 * @note	Run as: valgrind --leak-check=yes --quiet ./w3ctest
 */ 

DeclareClass( W3Ctest, TestCase );

Virtuals( W3Ctest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( W3Ctest, TestCase )

	/* The test case */
	char *		testCaseFileName;
	FILE *		testCaseFile;
	XmlNode		testCaseXml;

	/* Individual tests */
	XmlNode				currentTest;
	const char *		testFileName;
	const char *		testName;
	char *				testable;
	XmlNode				parsedXml;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( W3Ctest, TestCase );


/* Class initializing
 */

static void w3ctest_before_class( W3Ctest self );
static void w3ctest_before( W3Ctest self );
static void w3ctest_after( W3Ctest self );
static void w3ctest_after_class( W3Ctest self );

static
void
W3Ctest_initialize( Class this )
{
	W3CtestVtable vtab = & W3CtestVtableInstance;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) w3ctest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) w3ctest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) w3ctest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) w3ctest_after_class;

	ooc_init_class( Exception );
	ooc_init_class( XmlNode );
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE

static
void
W3Ctest_finalize( Class this )
{
}

#endif

/* Constructor
 */

static
void
W3Ctest_constructor( W3Ctest self, const void * params )
{
	assert( ooc_isInitialized( W3Ctest ) );
	
	chain_constructor( W3Ctest, self, NULL );

	self->testCaseFileName = ooc_strdup( (char *) params );
}

/* Destructor
 */

static
void
W3Ctest_destructor( W3Ctest self, W3CtestVtable vtab )
{
	ooc_free_and_null( (void**) & self->testCaseFileName );
}

/* Copy constuctor
 */

static
int
W3Ctest_copy( W3Ctest self, const W3Ctest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */

#define BIGGEST_W3C_FILESIZE 256000L
static
void
w3ctest_before_class( W3Ctest self )
{
	unsigned long int 	fileSize;
	char *				buffer;
	size_t				bytes_read;
	XmlParser			parser;


	self->testCaseFile = fopen( self->testCaseFileName, "rb" );

	assertNotNullMsg( self->testCaseFile, "Can not open w3c XML test case file.");
	if( self->testCaseFile != NULL )
	{
		fseek( self->testCaseFile, 0L, SEEK_END );
		fileSize = ftell( self->testCaseFile );
		rewind( self->testCaseFile );

		assertTrueMsg( fileSize <= BIGGEST_W3C_FILESIZE, "w3c test case file is too big!" );
		assertTrueMsg( fileSize > 0, "w3c test case file is empty!" );
		if( fileSize <= BIGGEST_W3C_FILESIZE && fileSize > 0 )
		{
			buffer = ooc_malloc( fileSize + 1 );
			bytes_read = fread( buffer, 1, fileSize, self->testCaseFile );
			buffer[bytes_read] = '\0';

			parser = xmlparser_new_str( buffer );
			{
				ooc_manage_object( parser );
				self->testCaseXml = xmlnode_parse( parser );
				ooc_delete( (Object) ooc_pass( parser ) );
			}
		}
	}
}

static
void
w3ctest_before( W3Ctest self )
{
	char * 				fileName;
	FILE *				testFile;
	unsigned long int 	fileSize;
	size_t				bytes_read;

	fileName = ooc_malloc( strlen(self->testCaseFileName) + strlen("xmltest/") + 1 );
	strcpy( fileName, "xmltest/" );
	strcpy( fileName+strlen(fileName), self->testFileName );
	testFile = fopen( fileName, "rb" );
	ooc_free( fileName );

	assertNotNullMsg( testFile, "Can not open w3c XML test file.");
	if( testFile != NULL )
	{
		ooc_manage( testFile, (ooc_destroyer) fclose );

		fseek( testFile, 0L, SEEK_END );
		fileSize = ftell( testFile );
		rewind( testFile );

		assertTrueMsg( fileSize <= BIGGEST_W3C_FILESIZE, "w3c test file is too big!" );
		if( fileSize <= BIGGEST_W3C_FILESIZE )
		{
			self->testable = ooc_malloc( fileSize + 1 );
			bytes_read = fread( self->testable, 1, fileSize, testFile );
			self->testable[bytes_read] = '\0';

		};

		fclose( ooc_pass( testFile ) );
	}
}

static
void
w3ctest_after( W3Ctest self )
{
	ooc_free_and_null( (void**) & self->testable );
	ooc_delete_and_null( (Object*) & self->parsedXml );
}

static
void
w3ctest_after_class( W3Ctest self )
{
	FILE * tmpFile;

	tmpFile = ooc_ptr_read_and_null( (void**) & self->testCaseFile );
	if( tmpFile != NULL )
		fclose( tmpFile );
	ooc_delete_and_null( (Object*) & self->testCaseXml );
}

/* Printing the diag message
 */

static
void
w3ctest_print_text( XmlNode node )
{
	assert( ooc_isInstanceOf( node, XmlNode ) );

	if( xmlnode_get_type( node ) == XML_NODE_TEXT )
	{
		const char * value = xmlnode_get_value( node );
		if( value )
			printf( "%s", value );
	}
	else
	{
		List children = (List) xmlnode_get_children( node );
		if( children )
			list_foreach( children, (list_item_executor) w3ctest_print_text, NULL );
	}
}

static
void
w3ctest_print_diag( XmlNode test )
{
	const char * sections;

	assert( ooc_isInstanceOf( test, XmlNode ) );
	assert( xmlnode_get_type( test ) == XML_NODE_ELEMENT );
	assert( strcmp( xmlnode_get_name( test ), "TEST" ) == 0 );

	if( ( sections = xmlnode_get_attrib( test, "SECTIONS" ) ) )
		printf( "\tSections %s\n", sections );

	list_foreach( (List) xmlnode_get_children( test ), (list_item_executor) w3ctest_print_text, NULL );
	printf( "\n" );
}


/* ===========================================================
 * Test cycles
 */

static
void
w3ctest_test_well_formed( W3Ctest self )
{
	XmlParser	parser;

	parser = xmlparser_new_str( ooc_strdup( self->testable ) );

	try
		ooc_delete( (Object) xmlnode_parse( parser ) ) ;
	catch( XmlException )
	{
		fail();
		w3ctest_print_diag( self->currentTest );
	}
	finally
		ooc_delete( (Object) parser );
	end_try;
}

static
void
w3ctest_test_not_well_formed( W3Ctest self )
{
	XmlParser	parser;

	parser = xmlparser_new_str( ooc_strdup( self->testable ) );

	try {
		ooc_delete( (Object) xmlnode_parse( parser ) ) ;
		fail();
		w3ctest_print_diag( self->currentTest );
	}
	catch( XmlException ) {
	}
	finally
		ooc_delete( (Object) parser );
	end_try;
}

static
void
w3ctest_run_child( XmlNode currentNode, W3Ctest testCase )
{
	if( xmlnode_get_type( currentNode ) == XML_NODE_ELEMENT )
	{
		if( strcmp( xmlnode_get_name( currentNode ), "TEST" ) == 0 )
		{
			testCase->currentTest 	= currentNode;
			const char * test_type	= xmlnode_get_attrib( currentNode, "TYPE" );
			testCase->testFileName	= xmlnode_get_attrib( currentNode, "URI" );
			testCase->testName		= xmlnode_get_attrib( currentNode, "ID" );

			assertNotNullMsg( test_type, "Test has no TYPE attribute!");
			if( test_type )
			{
				if( strcmp( test_type, "valid") == 0
					|| strcmp( test_type, "invalid") == 0
					|| strcmp( test_type, "error") == 0 )
					testcase_run_test( (TestCase) testCase, testCase->testName, (test_method_type) w3ctest_test_well_formed );
				else if( strcmp( test_type, "not-wf") == 0 )
					testcase_run_test( (TestCase) testCase, testCase->testName, (test_method_type) w3ctest_test_not_well_formed );
				else
				{
					fail();
					printf("Test TYPE %s is not established.\n", test_type );
				}
			}
		}
		else
			list_foreach( xmlnode_get_children( currentNode ), (list_item_executor) w3ctest_run_child, testCase );
	}
}

static
void
w3ctest_run_all( W3Ctest self )
{
	List	children;

	children = xmlnode_get_children( self->testCaseXml );

	list_foreach( children, (list_item_executor) w3ctest_run_child, self );
}

/* Runs the test as an executable
 */
 
TESTCASE_MAIN
{
	W3Ctest w3ctest;
	int result;
	
	ooc_init_class( W3Ctest );

	w3ctest = ooc_new( W3Ctest, "xmltest/xmltest_mod.xml" );

	if( testcase_run_before_class( (TestCase) w3ctest ) == 0 )
		w3ctest_run_all( w3ctest );
	result = testcase_run_after_class( (TestCase) w3ctest);

	ooc_delete( (Object) w3ctest );
	ooc_finalize_all();
	return result;
}
