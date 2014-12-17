
/* This is a ProbaTest class implementation file
 */

#include "../libs/testcase.h"

#ifdef OOC_NO_FINALIZE
#define ooc_finalize_class( x )
#define ooc_finalize_all( )
#endif

/** @class SignalTest
 *  @brief SignalTest class - brief description.
 * 	@see probatest.h
 */

/** @file probatest.h 
 *  @brief SignalTest class - brief description.
 * SignalTest is a .....
 * @note	This class is a final class, can not be inherited.
 */ 

DeclareClass( TestCaseTest, TestCase );

Virtuals( TestCaseTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( TestCaseTest, TestCase )

	int		data;
	
	int		before_class_counter;
	int		before_counter;
	int		after_counter;
	int		after_class_counter;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( TestCaseTest, TestCase );


/* Class initializing
 */

static void testcasetest_before_class( TestCaseTest self );
static void testcasetest_before( TestCaseTest self );
static void testcasetest_after( TestCaseTest self );
static void testcasetest_after_class( TestCaseTest self );

static
void
TestCaseTest_initialize( Class this )
{
	TestCaseTestVtable vtab = (TestCaseTestVtable) this->vtable;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) testcasetest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) testcasetest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) testcasetest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) testcasetest_after_class;

	ooc_init_class( Exception );
	
	/* Allocate global resources here */
}

/* Class finalizing
 */
#ifndef OOC_NO_FINALIZE
static
void
TestCaseTest_finalize( Class this )
{
	/* Release global resources! */
}
#endif

/* Constructor
 */

static
void
TestCaseTest_constructor( TestCaseTest self, const void * params )
{
	assert( ooc_isInitialized( TestCaseTest ) );
	
	chain_constructor( TestCaseTest, self, params );
}

/* Destructor
 */

static
void
TestCaseTest_destructor( TestCaseTest self, TestCaseTestVtable vtab )
{
}

/* Copy constuctor
 */

static
int
TestCaseTest_copy( TestCaseTest self, const TestCaseTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
testcasetest_before_class( TestCaseTest self )
{
	self->before_class_counter++;
}

static
void
testcasetest_before( TestCaseTest self )
{
	self->before_counter++;
}

static
void
testcasetest_after( TestCaseTest self )
{
	self->after_counter++;
}

static
void
testcasetest_after_class( TestCaseTest self )
{
	self->after_class_counter++;
}


/*	=====================================================
	Test methods
 */

#ifdef OOC_HAS_UNIX_SIGNALS

void
static
segfault( TestCaseTest self )
{
	/* faking optimizing compilers: bad_ptr is volatile! */
	volatile int * bad_ptr = NULL;

	try {
		int i = *bad_ptr;
		failMsg( "Can fail in Valgrind!" );
	}
	catch_any {
		assertTrue( ooc_isInstanceOf( exception, SegmentationFault ) );
	}
	end_try;
}

void
static
fpfault( TestCaseTest self )
{
	/* faking optmizing compilers: zero is volatile! */
	volatile int zero = 0;

	try {
		zero = zero / zero;
		fail();
	}
	catch_any {
		assertTrue( ooc_isInstanceOf( exception, ArithmeticFault ) );
	}
	end_try;	
}

void
static
callnull( TestCaseTest self )
{
	void (* bad_ptr)(void) = NULL;
	
	try {
		bad_ptr(); /* may result a real segmentation fault in some cases :-( I don't understand ... :-( */
		fail();
	}
	catch_any {
		assertTrue( ooc_isInstanceOf( exception, SegmentationFault ) );
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
	TEST(segfault),
	TEST(fpfault),
	TEST(callnull),
	
	{NULL, NULL} /* Do NOT delete this line! */
};

typedef struct TestCaseObject * TestCase;

static
int
run_testcasetest( void )
{
	struct TestCaseTestObject testcasetesto;
	int result;
	
	ooc_init_class( TestCaseTest );
	ooc_use( &testcasetesto, TestCaseTest, (void*) &methods );
	result = testcase_run((TestCase) &testcasetesto);
	ooc_release( (Object) &testcasetesto );
	ooc_finalize_all();
	
	return result;
}

#endif

/*****************************************************
 * Testing the derived TestCase class
 */
 
DeclareClass( TestCaseTestChild, TestCaseTest );

Virtuals( TestCaseTestChild, TestCaseTest )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( TestCaseTestChild, TestCaseTest )

	int		before_class_counter;
	int		before_counter;
	int		after_counter;
	int		after_class_counter;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( TestCaseTestChild, TestCaseTest );


/* Class initializing
 */

static void testcasetestchild_before_class( TestCaseTestChild self );
static void testcasetestchild_before( TestCaseTestChild self );
static void testcasetestchild_after( TestCaseTestChild self );
static void testcasetestchild_after_class( TestCaseTestChild self );

static
void
TestCaseTestChild_initialize( Class this )
{
	TestCaseVtable vtab = (TestCaseVtable) this->vtable;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) testcasetestchild_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) testcasetestchild_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) testcasetestchild_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) testcasetestchild_after_class;

	ooc_init_class( Exception );
}

/* Class finalizing
 */
#ifndef OOC_NO_FINALIZE
static
void
TestCaseTestChild_finalize( Class this )
{
}
#endif

/* Constructor
 */

static
void
TestCaseTestChild_constructor( TestCaseTestChild self, const void * params )
{
	assert( ooc_isInitialized( TestCaseTestChild ) );
	
	chain_constructor( TestCaseTestChild, self, params );
}

/* Destructor
 */

static
void
TestCaseTestChild_destructor( TestCaseTestChild self, TestCaseTestChildVtable vtab )
{
}

/* Copy constuctor
 */

static
int
TestCaseTestChild_copy( TestCaseTestChild self, const TestCaseTestChild from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
testcasetestchild_before_class( TestCaseTestChild self )
{
	assertTrue( self->TestCaseTest.before_class_counter == 1 );
	
	self->before_class_counter++;
}

static
void
testcasetestchild_before( TestCaseTestChild self )
{
	assertTrue( self->TestCaseTest.before_counter == self->before_counter + 1 );
	
	self->before_counter++;
}

static
void
testcasetestchild_after( TestCaseTestChild self )
{
	self->after_counter++;
	
	assertTrue( self->TestCaseTest.after_counter == self->after_counter - 1 );	
}

static
void
testcasetestchild_after_class( TestCaseTestChild self )
{
	assertTrue( self->TestCaseTest.after_class_counter == 0 );
	
	self->after_class_counter++;
}


/*	=====================================================
	Test methods
 */

void
static
testcasetestchild_first_run( TestCaseTestChild self )
{
	assertTrue( self->TestCaseTest.before_class_counter == 1 );
	assertTrue( self->TestCaseTest.before_counter == 1 );
	assertTrue( self->TestCaseTest.after_counter == 0 );
	assertTrue( self->TestCaseTest.after_class_counter == 0 );
	
	assertTrue( self->before_class_counter == 1 );
	assertTrue( self->before_counter == 1 );
	assertTrue( self->after_counter == 0 );
	assertTrue( self->after_class_counter == 0 );
}

void
static
testcasetestchild_second_run( TestCaseTestChild self )
{
	assertTrue( self->TestCaseTest.before_class_counter == 1 );
	assertTrue( self->TestCaseTest.before_counter == 2 );
	assertTrue( self->TestCaseTest.after_counter == 1 );
	assertTrue( self->TestCaseTest.after_class_counter == 0 );

	assertTrue( self->before_class_counter == 1 );
	assertTrue( self->before_counter == 2 );
	assertTrue( self->after_counter == 1 );
	assertTrue( self->after_class_counter == 0 );
}

void
static
testcasetestchild_third_run( TestCaseTestChild self )
{
	assertTrue( self->TestCaseTest.before_class_counter == 1 );
	assertTrue( self->TestCaseTest.before_counter == 3 );
	assertTrue( self->TestCaseTest.after_counter == 2 );
	assertTrue( self->TestCaseTest.after_class_counter == 0 );
	
	assertTrue( self->before_class_counter == 1 );
	assertTrue( self->before_counter == 3 );
	assertTrue( self->after_counter == 2 );
	assertTrue( self->after_class_counter == 0 );	
}

/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
ROM struct TestCaseMethod childmethods[] =
{
	
	TEST(testcasetestchild_first_run),
	TEST(testcasetestchild_second_run),
	TEST(testcasetestchild_third_run),
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */

typedef struct TestCaseObject * TestCase;

static
int
run_testcasetestchild( void )
{
	struct TestCaseTestChildObject testcasetestchildo;
	int result;
	
	ooc_init_class( TestCaseTestChild );
	ooc_use( &testcasetestchildo, TestCaseTestChild, (void*) &childmethods );
	result = testcase_run((TestCase) &testcasetestchildo);
	
	assertTrue( testcasetestchildo.before_class_counter == 1 );
	assertTrue( testcasetestchildo.TestCaseTest.before_class_counter == 1 );
	assertTrue( testcasetestchildo.after_class_counter == 1 );
	assertTrue( testcasetestchildo.TestCaseTest.after_class_counter == 1 );
	
	ooc_release( (Object) &testcasetestchildo );
	ooc_finalize_all();
	return result;
}

/* Runs the test as an executable
 */
 
TESTCASE_MAIN
{
	int result = 0;
	
#ifdef OOC_HAS_UNIX_SIGNALS
	result = run_testcasetest();
#endif
	
	if( result == 0 )
		result = run_testcasetestchild();
	
	abort();
}



