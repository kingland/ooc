
/* This is a RefCountedTest class implementation file
 */

#include "../libs/testcase.h"

#include "../libs/refcounted.h"
#include "../libs/implement/refcounted.h"

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

#ifdef OOC_NO_FINALIZE
#define ooc_finalize_class( x )
#define ooc_finalize_all( )
#endif

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
	RefCountedTestVtable vtab = & RefCountedTestVtableInstance;
	
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
	FooRefCnt foo;
	
	ooc_init_class( FooRefCnt );
	
	foo = (FooRefCnt) ooc_new( FooRefCnt, NULL );
	
	assertTrue( foo->RefCounted.counter == 1 );
	
	ooc_delete( (Object) foo );
	
	ooc_finalize_class( FooRefCnt );
}

void
static
test_addref( void )
{
	FooRefCnt foo;
	
	ooc_init_class( FooRefCnt );
	
	foo = (FooRefCnt) ooc_new( FooRefCnt, NULL );
	foo_destructor_counter = 0;
	
	assertTrue( foo->RefCounted.counter == 1 );
	
	assertTrue( refcnt_addref( (RefCounted) foo ) == (RefCounted) foo );
	assertTrue( foo->RefCounted.counter == 2 );
	
	assertTrue( refcnt_inc( foo ) == foo );
	assertTrue( foo->RefCounted.counter == 3 );
	
	ooc_delete( (Object) foo );
	assertTrue( foo->RefCounted.counter == 2 );
	
	ooc_delete( (Object) foo );
	assertTrue( foo->RefCounted.counter == 1 );
	
	ooc_delete( (Object) foo );
	
	assertTrue( foo_destructor_counter == 1 );
	
	ooc_finalize_class( FooRefCnt );
}

#define TEST_CYCLE_NUMBER 10000

static
void
test_multithread( void ) 
{
	FooRefCnt	foo;
	int			i;
	
	ooc_init_class( FooRefCnt );
	
	foo = (FooRefCnt) ooc_new( FooRefCnt, NULL );
	foo_destructor_counter = 0;
	assertTrue( foo->RefCounted.counter == 1 );
	
	#ifdef _OPENMP
	#pragma omp parallel for private(i) schedule( guided, 10 )
	#endif
	for( i = 0; i<TEST_CYCLE_NUMBER; i++ )
		refcnt_addref( (RefCounted) foo );
	
	assertTrue( foo->RefCounted.counter == TEST_CYCLE_NUMBER +1 );
	
	for( i = 0; i<TEST_CYCLE_NUMBER; i++ )
		ooc_delete( (Object) foo );

	assertTrue( foo->RefCounted.counter == 1 );
	assertTrue( foo_destructor_counter == 0 );
		
	ooc_delete( (Object) foo );
		
	assertTrue( foo_destructor_counter == 1 );
	
	ooc_finalize_class( FooRefCnt );
}

/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
ROM_ALLOC
struct TestCaseMethod methods[] =
{
	
	TEST(test_constructor),
	TEST(test_addref),
	TEST(test_multithread),
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
TESTCASE_MAIN
{
	RefCountedTest refcountedtest;
	int result;
	
	ooc_init_class( RefCountedTest );
	refcountedtest = (RefCountedTest) ooc_new( RefCountedTest, &methods );
	result = testcase_run((TestCase)refcountedtest);
	ooc_delete( (Object) refcountedtest );
	ooc_finalize_all();
	return result;
}


