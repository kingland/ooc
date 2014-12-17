
/* This is a ExceptionTest class implementation file
 */

#include "../libs/testcase.h"

#include "foo.h"
#include "implement/foo.h"
#include "fooexception.h"

/** @class ExceptionTest
 *  @brief ExceptionTest class - brief description.
 * 	@see exceptiontest.h
 */

/** @file exceptiontest.h 
 *  @brief ExceptionTest class - brief description.
 * ExceptionTest is a .....
 * @note	run as: valgrind --leak-check=yes --quiet ./exceptiontest
 * @note	This class is a final class, can not be inherited.
 */ 

DeclareClass( ExceptionTest, TestCase );

Virtuals( ExceptionTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( ExceptionTest, TestCase )

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( ExceptionTest, TestCase );


/* Class initializing
 */

static void exceptiontest_before_class( ExceptionTest self );
static void exceptiontest_before( ExceptionTest self );
static void exceptiontest_after( ExceptionTest self );
static void exceptiontest_after_class( ExceptionTest self );

static
void
ExceptionTest_initialize( Class this )
{
	ExceptionTestVtable vtab = (ExceptionTestVtable) this->vtable;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) exceptiontest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) exceptiontest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) exceptiontest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) exceptiontest_after_class;

	ooc_init_class( FooException );

	/* Allocate global resources here */
}

/* Class finalizing
 */
#ifndef OOC_NO_FINALIZE
static
void
ExceptionTest_finalize( Class this )
{
	/* Release global resources! */
}
#endif


/* Constructor
 */

static
void
ExceptionTest_constructor( ExceptionTest self, const void * params )
{
	assert( ooc_isInitialized( ExceptionTest ) );
	
	chain_constructor( ExceptionTest, self, params );
}

/* Destructor
 */

static
void
ExceptionTest_destructor( ExceptionTest self, ExceptionTestVtable vtab )
{
}

/* Copy constuctor
 */

static
int
ExceptionTest_copy( ExceptionTest self, const ExceptionTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
exceptiontest_before_class( ExceptionTest self )
{
	return;
}

static
void
exceptiontest_before( ExceptionTest self )
{
	return;
}

static
void
exceptiontest_after( ExceptionTest self )
{
	return;
}

static
void
exceptiontest_after_class( ExceptionTest self )
{
	return;
}


/*	=====================================================
	Test methods
 */

void
static
test_constructor( ExceptionTest self )
{
	Exception exception;

	exception = exception_new( err_bad_throw );

	assertNotNull( exception );
	assertTrue( ooc_isInstanceOf( exception, Exception ) );
	assertTrue( exception_get_error_code( exception ) == err_bad_throw );
	assertTrue( exception_get_user_code( exception ) == 0 );

	ooc_release( (Object) exception );
}

void
static
test_inheritance( ExceptionTest self )
{
	Exception exception;

	exception = foo_exception_new( 13 );

	assertNotNull( exception );
	assertTrue( ooc_isInstanceOf( exception, FooException ) );
	assertTrue( ooc_isInstanceOf( exception, Exception ) );
	assertTrue( exception_get_error_code( exception ) == err_user_code );
	assertTrue( exception_get_user_code( exception ) == 13 );

	ooc_release( (Object) exception );
}

static
void
test_throw( void )
{
	Exception fooexception = foo_exception_new( 13 );
	
	try {
		ooc_throw( fooexception );
		fail();
	}
	catch( FooException ) {
		assertTrue( ooc_isInstanceOf( exception, FooException ) );
		assertTrue( exception == fooexception );
	}
	end_try;
}


static
void
test_rethrow( void )
{
	int volatile inner_finalized = FALSE;
	int volatile outer_finalized = FALSE;
	
	try {
		try {
			ooc_throw( (Exception) foo_exception_new( 13 ) );
		}
		catch( FooException ) {
			ooc_rethrow();
		}
		finally {
			inner_finalized = TRUE;
		}
		end_try;
	}
	catch_any {
		assertTrue( ooc_isInstanceOf( exception, FooException ) );
		assertTrue( exception_get_user_code( exception ) == 13 );
	}
	finally {
		outer_finalized = TRUE;
	}
	end_try;
	
	assertTrue( inner_finalized );
	assertTrue( outer_finalized );
}

static
void
test_throw_different( void )
{
	int volatile inner_finalized = FALSE;
	int volatile outer_finalized = FALSE;
	
	#define FIRST 13
	#define SECOND 26
	
	try {
		try {
			ooc_throw( foo_exception_new( FIRST ) );
		}
		catch( FooException ) {
			assertTrue( exception_get_user_code( exception ) == FIRST );
			ooc_throw( foo_exception_new( SECOND ));
		}
		finally {
			inner_finalized = TRUE;
		}
		end_try;
	}
	catch_any {
		assertTrue( exception_get_user_code( exception ) == SECOND );
	}
	finally {
		outer_finalized = TRUE;
	}
	end_try;
	
	assertTrue( inner_finalized );
	assertTrue( outer_finalized );	
}

#define MANAGE_TEST_EXCEPTION_ID 1111

static
void
manage_test_2( void )
{
	struct FooObject 	foo1o;
	struct FooObject 	foo2o;
	
	foo_use( &foo1o );
	{
		ooc_manage( &foo1o, (ooc_destroyer) ooc_release );
		
		foo_use( &foo2o );
		{
			ooc_manage( &foo2o, (ooc_destroyer) ooc_release  );
			ooc_throw( foo_exception_new( MANAGE_TEST_EXCEPTION_ID ) );
			ooc_release( (Object) ooc_pass( &foo2o ) );
		}
		ooc_release( (Object) ooc_pass( &foo1o ) );
	}
}

static
void
manage_test_1( void )
{
	struct FooObject 	foo1o;
	struct FooObject 	foo2o;
	
	foo_use( &foo1o );
	{
		ooc_manage( &foo1o, (ooc_destroyer) ooc_release );
		
		foo_use( &foo2o );
		{
			ooc_manage( &foo2o, (ooc_destroyer) ooc_release  );
			manage_test_2();
			ooc_release( (Object) ooc_pass( &foo2o ) );
		}
		ooc_release( (Object) ooc_pass( &foo1o ) );
	}
}

static
void
test_manage( void )
{
	try {
		manage_test_1( );
		fail();
		}
	catch( FooException ) {
		assertTrue( exception_get_user_code(exception) == MANAGE_TEST_EXCEPTION_ID );
		}
	end_try;
}

/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
ROM struct TestCaseMethod methods[] =
{
	
	TEST(test_constructor),
	TEST(test_inheritance),
	TEST(test_throw),
	TEST(test_rethrow),
	TEST(test_throw_different),
	TEST(test_manage),
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */

typedef struct TestCaseObject * TestCase;
 
TESTCASE_MAIN
{
	static struct ExceptionTestObject exceptiontesto;
	int result;
	
	ooc_init_class( ExceptionTest );
	ooc_use( &exceptiontesto, ExceptionTest, (void*) &methods );
	result = testcase_run((TestCase) &exceptiontesto);
	ooc_release( (Object) &exceptiontesto );
#ifndef OOC_NO_FINALIZE
	ooc_finalize_all();
#endif
	
	abort();
}



