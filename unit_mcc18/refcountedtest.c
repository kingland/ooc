
/* This is a RefCountedTest class implementation file
 */


#ifdef _OPENMP
#include <omp.h>
#endif

#include "testcase.h"

#include "refcounted.h"
#include "implement/refcounted.h"

#ifdef OOC_NO_FINALIZE
#define ooc_finalize_class( x )
#define ooc_finalize_all( )
#endif


/** @class RefCountedTest
 *  @brief RefCountedTest class - brief description.
 * 	@see refcountedtest.h
 */

/** @file refcountedtest.h 
 *  @brief RefCountedTest class - brief description.
 * RefCountedTest is a .....
 * @note	This class is a final class, can not be inherited.
 * @note	Run as: valgrind --leak-check=yes --quiet ./refcountedtest
 */ 

DeclareClass( RefCountedTest, TestCase );

Virtuals( RefCountedTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( RefCountedTest, TestCase )

	int		data;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( RefCountedTest, TestCase );


/* Class initializing
 */

static void refcountedtest_before_class( RefCountedTest self );
static void refcountedtest_before( RefCountedTest self );
static void refcountedtest_after( RefCountedTest self );
static void refcountedtest_after_class( RefCountedTest self );

static
void
RefCountedTest_initialize( Class this )
{
	RefCountedTestVtable vtab = (RefCountedTestVtable) this->vtable;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) refcountedtest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) refcountedtest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) refcountedtest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) refcountedtest_after_class;

	ooc_init_class( Exception );
	
	/* Allocate global resources here */
}

/* Class finalizing
 */
#ifndef OOC_NO_FINALIZE
static
void
RefCountedTest_finalize( Class this )
{
	/* Release global resources! */
}
#endif

/* Constructor
 */

static
void
RefCountedTest_constructor( RefCountedTest self, const void * params )
{
	assert( ooc_isInitialized( RefCountedTest ) );
	
	chain_constructor( RefCountedTest, self, params );
}

/* Destructor
 */

static
void
RefCountedTest_destructor( RefCountedTest self, RefCountedTestVtable vtab )
{
}

/* Copy constuctor
 */

static
int
RefCountedTest_copy( RefCountedTest self, const RefCountedTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
refcountedtest_before_class( RefCountedTest self )
{
	return;
}

static
void
refcountedtest_before( RefCountedTest self )
{
	return;
}

static
void
refcountedtest_after( RefCountedTest self )
{
	return;
}

static
void
refcountedtest_after_class( RefCountedTest self )
{
	return;
}


/*	=====================================================
	Test methods
 */

DeclareClass( FooRefCnt, RefCounted );

ClassMembers( FooRefCnt, RefCounted )
EndOfClassMembers;

Virtuals( FooRefCnt, RefCounted )
EndOfVirtuals;

AllocateClass( FooRefCnt, RefCounted );

typedef struct RefCountedObject * RefCounted;

static	void	FooRefCnt_initialize( Class this ) {}
#ifndef OOC_NO_FINALIZE
static	void	FooRefCnt_finalize( Class this ) {}
#endif

static	void	FooRefCnt_constructor( FooRefCnt self, const void * params )
{
	assert( ooc_isInitialized( FooRefCnt ) );
	
	chain_constructor( FooRefCnt, self, NULL );
}

static	int		foo_destructor_counter;
static	void	FooRefCnt_destructor( FooRefCnt self, FooRefCntVtable vtab )
{
	assertNull( self->RefCounted.Base._vtab );
	++foo_destructor_counter;
}
static	int		FooRefCnt_copy( FooRefCnt self, const FooRefCnt from ) { return OOC_COPY_DEFAULT; }


void
static
test_constructor( void )
{
	static struct FooRefCntObject foo_o;
	
	ooc_init_class( FooRefCnt );
	
	ooc_use( & foo_o, FooRefCnt, NULL );
	
	assertTrue( foo_o.RefCounted.counter == 1 );
	
	ooc_release( (Object) & foo_o );
	
	ooc_finalize_class( FooRefCnt );
}

void
static
test_addref( void )
{
	static struct FooRefCntObject foo_o;
	
	ooc_init_class( FooRefCnt );
	
	ooc_use( & foo_o, FooRefCnt, NULL );
	foo_destructor_counter = 0;
	
	assertTrue( foo_o.RefCounted.counter == 1 );
	
	assertTrue( refcnt_addref( (RefCounted) &foo_o ) == (RefCounted) &foo_o );
	assertTrue( foo_o.RefCounted.counter == 2 );
	
	assertTrue( refcnt_inc( &foo_o ) == &foo_o );
	assertTrue( foo_o.RefCounted.counter == 3 );
	
	ooc_release( (Object) &foo_o );
	assertTrue( foo_o.RefCounted.counter == 2 );
	
	ooc_release( (Object) &foo_o );
	assertTrue( foo_o.RefCounted.counter == 1 );
	
	ooc_release( (Object) &foo_o );
	
	assertTrue( foo_destructor_counter == 1 );
	
	ooc_finalize_class( FooRefCnt );
}

#define TEST_CYCLE_NUMBER 13

static
void
test_multithread( void ) 
{
	static struct FooRefCntObject foo_o;
	int			i;
	
	ooc_init_class( FooRefCnt );
	
	ooc_use( & foo_o, FooRefCnt, NULL );
	foo_destructor_counter = 0;
	assertTrue( foo_o.RefCounted.counter == 1 );
	
	#ifdef _OPENMP
	#pragma omp parallel for private(i) schedule( guided, 10 )
	#endif
	for( i = 0; i<TEST_CYCLE_NUMBER; i++ )
		refcnt_addref( (RefCounted) &foo_o );
	
	assertTrue( foo_o.RefCounted.counter == TEST_CYCLE_NUMBER +1 );
	
	for( i = 0; i<TEST_CYCLE_NUMBER; i++ )
		ooc_release( (Object) &foo_o );

	assertTrue( foo_o.RefCounted.counter == 1 );
	assertTrue( foo_destructor_counter == 0 );
		
	ooc_release( (Object) &foo_o );
		
	assertTrue( foo_destructor_counter == 1 );
	
	ooc_finalize_class( FooRefCnt );
}

/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
ROM struct TestCaseMethod methods[] =
{
	
	TEST(test_constructor),
	TEST(test_addref),
	TEST(test_multithread),
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */

typedef struct TestCaseObject * TestCase;
 
TESTCASE_MAIN
{
	struct RefCountedTestObject refcountedtesto;
	int result;
	
	ooc_init_class( RefCountedTest );
	ooc_use( &refcountedtesto, RefCountedTest, (void*) &methods );
	result = testcase_run((TestCase) &refcountedtesto);
	ooc_release( (Object) &refcountedtesto );
	ooc_finalize_all();

	abort();
}


