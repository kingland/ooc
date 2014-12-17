
/* This is a VectorTest class implementation file
 */

#include "../libs/testcase.h"

/* Testable */
#include "../libs/vector.h"

#include "foo.h"
#include "bar.h"
#include "barson.h"
#include "bardaughter.h"

#ifdef OOC_NO_FINALIZE
#define ooc_finalize_class( x )
#define ooc_finalize_all( )
#endif

/** @class VectorTest
 *  @brief VectorTest class - brief description.
 * 	@see vectortest.h
 */

/** @file vectortest.h 
 *  @brief VectorTest class - brief description.
 * VectorTest is a .....
 * @note	This class is a final class, can not be inherited.
 * @note	Run as: valgrind --leak-check=yes --quiet ./vectortest
 */ 

DeclareClass( VectorTest, TestCase );

Virtuals( VectorTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( VectorTest, TestCase )

	int		data;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( VectorTest, TestCase );


/* Class initializing
 */

static void vectortest_before_class( VectorTest self );
static void vectortest_before( VectorTest self );
static void vectortest_after( VectorTest self );
static void vectortest_after_class( VectorTest self );

static
void
VectorTest_initialize( Class this )
{
	VectorTestVtable vtab = & VectorTestVtableInstance;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) vectortest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) vectortest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) vectortest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) vectortest_after_class;

	ooc_init_class( Exception );
	ooc_init_class( Vector );

	ooc_init_class( Foo );
	ooc_init_class( Bar );
	ooc_init_class( BarSon );
	ooc_init_class( BarDaughter );

	/* Allocate global resources here */
}

/* Class finalizing#include "../libs/ooc.h"
#include "../libs/exception.h"
 * 
 */

#ifndef OOC_NO_FINALIZE

static
void
VectorTest_finalize( Class this )
{
	/* Release global resources! */
}

#endif

/* Constructor
 */

static
void
VectorTest_constructor( VectorTest self, const void * params )
{
	assert( ooc_isInitialized( VectorTest ) );
	
	chain_constructor( VectorTest, self, params );
}

/* Destructor
 */

static
void
VectorTest_destructor( VectorTest self, VectorTestVtable vtab )
{
}

/* Copy constuctor
 */

static
int
VectorTest_copy( VectorTest self, const VectorTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
vectortest_before_class( VectorTest self )
{
	return;
}

static
void
vectortest_before( VectorTest self )
{
	return;
}

static
void
vectortest_after( VectorTest self )
{
	return;
}

static
void
vectortest_after_class( VectorTest self )
{
	return;
}


/*	=====================================================
	Test methods
 */

static
void
test_pushback( void )
{
	Vector vector = vector_new_type( 10, Foo, TRUE );
	int i;
	
	#ifdef _OPENMP
	#pragma omp parallel for private(i) schedule( guided, 10 )
	#endif
	for( i = 0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( i ) );
		
	assertTrue( vector_items( vector ) == 100 );
	
	ooc_delete( (Object) vector );
}

static
void
test_string_vector( void )
{
	Vector vector = vector_new( 10, (vector_item_destroyer) ooc_free );
	int i;

	#ifdef _OPENMP
	#pragma omp parallel for private(i) schedule( guided, 10 )
	#endif
	for( i = 0; i<100; i++ )
		vector_push_back( vector, ooc_strdup( "Test string." ) );
		
	assertTrue( vector_items( vector ) == 100 );
	
	ooc_delete( (Object) vector );
}

static
void
test_untyped( void )
{
	Vector vector = vector_new( 10, (vector_item_destroyer) ooc_delete );

	assertNull( vector_get_type( vector ) );
	assertTrue( vector_get_managed( vector ) == OOC_MANAGE );

	/* Must NOT violate type safety
	 */
	vector_push_back( vector, bar_new( ) );
	vector_push_back( vector, foo_new( ) );
	vector_push_back( vector, barson_new( ) );
	vector_push_back( vector, bardaughter_new( ) );
		
	ooc_delete( (Object) vector );
}

static
void
test_type_safety_I( void )
{
	Vector vector = vector_new_type( 10, Foo, OOC_MANAGE );

	assertTrue( vector_get_type( vector ) == & FooClass );
	assertTrue( vector_get_managed( vector ) == OOC_MANAGE );

	/* Must violate type safety
	 */
	
	try { 
		vector_push_back( vector, bar_new_with_const_text( "bad") );
		fail();
		}
	catch_any {
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	}
	end_try;
	
	ooc_delete( (Object) vector );
}

static
void
test_type_safety_II( void )
{
	Vector vector = vector_new_type( 10, Bar, OOC_MANAGE );

	assertTrue( vector_get_type( vector ) == & BarClass );
	assertTrue( vector_get_managed( vector ) == OOC_MANAGE );

	/* Must NOT violate type safety
	 */
	vector_push_back( vector, barson_new( ) );
	vector_push_back( vector, bardaughter_new( ) );
		
	ooc_delete( (Object) vector );
}

static
void
test_type_safety_set_item( void )
{
	Vector vector;
	
	vector = vector_new_type( 10, Bar, OOC_MANAGE );
	vector_push_back( vector, bar_new( ) );
	try {
		vector_set_item( vector, 0, foo_new() );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;
	ooc_delete( (Object) vector );
	
	vector = vector_new( 10, (vector_item_destroyer) ooc_delete );
	vector_push_back( vector, bar_new( ) );
	vector_set_item( vector, 0, foo_new( ) );
	vector_set_item( vector, 0, bar_new( ) );
	ooc_delete( (Object) vector );
}

static
void
test_type_safety_insert( void )
{
	Vector vector;
	
	vector = vector_new_type( 10, Bar, OOC_MANAGE );
	vector_push_back( vector, bar_new( ) );
	try {
		vector_insert( vector, 0, foo_new() );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;
	ooc_delete( (Object) vector );
	
	vector = vector_new( 10, (vector_item_destroyer) ooc_delete );
	vector_push_back( vector, bar_new( ) );
	vector_insert( vector, 0, foo_new( ) );
	vector_insert( vector, 0, bar_new( ) );
	ooc_delete( (Object) vector );
}

static
void
test_from_table( void )
{
	static
	struct TestData
	{
		int				counter;
		const char * 	text;
	}
	testData[] =
	{
		{ 0, "text 0" },
		{ 1, "text 1" },
		{ 2, "text 2" },
		{ 3, "text 3" },
		{ 4, "text 4" },
		{ 5, "text 5" },
		{ 6, "text 6" },
	};
	#define RECORDS (sizeof(testData)/sizeof(struct TestData))
	
	Vector vector = vector_new_from_table( &testData, sizeof(struct TestData), RECORDS );
	int i;
	
	assertTrue( vector_get_managed( vector ) == !OOC_MANAGE );

	assertTrue( vector_items( vector ) == RECORDS );
	for( i = 0; i<RECORDS; i++ )
		assertTrue( vector_get_item( vector, i ) == &testData[i] );

	ooc_delete( (Object) vector );
}

static
void
test_set_item( void )
{
	Vector vector = vector_new_type( 10, Foo, OOC_MANAGE );
	int i;

	#ifdef _OPENMP
	#pragma omp parallel for private(i) schedule( guided, 10 )
	#endif
	for( i = 0; i<100; i++ )
		vector_push_back( vector, foo_new_with_const_text( "Test string." ) );
		
	assertTrue( vector_items( vector ) == 100 );
	
	#ifdef _OPENMP
	#pragma omp parallel for private(i) schedule( guided, 10 )
	#endif
	for( i = 0; i<100; i++ )
		vector_set_item( vector, i, foo_new_with_data( 100 - i ) );
		
	for( i = 0; i<100; i++ )
		assertTrue( foo_get_data( ooc_cast( vector_get_item( vector, i ), Foo ) ) ==  100 - i );
		
	ooc_delete( (Object) vector );
}

static
void
test_insert( void )
{
	Vector vector = vector_new_type( 10, Foo, OOC_MANAGE );
	int i;
	
	for(i=0; i<100; i++ )
		if( i % 7 != 0 )
			vector_push_back( vector, foo_new_with_data( i ) );

	for(i=0; i<100; i++ )
		if( i % 7 == 0 )
			vector_insert( vector, i, foo_new_with_data( i ) );

	for(i=0; i<100; i++ ) {
		Foo actual = ooc_cast( vector_get_item( vector, i ), Foo );
		assertTrue( foo_get_data( actual ) == i );
	}
	
	ooc_delete( (Object) vector );	
}

static
void
test_delete( void )
{
	Vector vector = vector_new_type( 10, Foo, OOC_MANAGE );
	VectorIndex i, j;
	
	for(i=0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( i ) );
	assertTrue( vector_items( vector ) == 100 );

	for(i=0; i < vector_items( vector ); )
		if( i % 7 == 0 )
			vector_delete_item( vector, i );
		else
			i++;
	assertTrue( vector_items( vector ) < 100 );

	for(i=0, j=0; i < vector_items( vector ); i++, j++ ) {
		Foo actual;
		
		if( j % 7 == 0 )
			j++;
			
		actual = ooc_cast( vector_get_item( vector, i ), Foo );
		assertTrue( foo_get_data( actual ) == j );
		}
				
	ooc_delete( (Object) vector );
}

static
void
test_swap( void )
{
	Vector vector = vector_new_type( 10, Foo, OOC_MANAGE );
	int i;
	
	for(i=0; i<50; i++ ) {
		vector_push_back( vector, foo_new_with_data( 2*i+1 ) );
		vector_push_back( vector, foo_new_with_data( 2*i ) );
		}
	assertTrue( vector_items( vector ) == 100 );

	for(i=0; i<50; i++ )
		vector_swap( vector, 2*i, 2*i+1 );
	assertTrue( vector_items( vector ) == 100 );
	
	for(i=0; i<100; i++ ) {
		Foo actual = ooc_cast( vector_get_item( vector, i ), Foo );
		assertTrue( foo_get_data( actual ) == i );
		}
	
	ooc_delete( (Object) vector );	
}

static char foreach_param[] = "parameter";
static int foreach_counter;

static
void
test_item_executor( Foo item, void * param )
{
	assertTrue( ooc_isInstanceOf( item, Foo ) );
	assertTrue( param == foreach_param );
	
	assertTrue( foo_get_data( item ) == foreach_counter );
	foreach_counter++;
}

static
void
test_foreach( void )
{
	Vector vector = vector_new_type( 10, Foo, OOC_MANAGE );
	int i;
	
	for(i=0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( i ) );
	assertTrue( vector_items( vector ) == 100 );

	foreach_counter = 0;
	vector_foreach( vector, (vector_item_executor) test_item_executor, &foreach_param );
	assertTrue( foreach_counter == vector_items( vector ) );
	
	ooc_delete( (Object) vector );	
}

static
int
item_is_17( Foo item, void * param )
{
	assertTrue( ooc_isInstanceOf( item, Foo ) );
	assertTrue( param == foreach_param );
	
	return ( foo_get_data( item ) == 17 ) ? TRUE : FALSE;
}

static
int
test_checker_always_false( Foo item, void * param )
{
	assertTrue( ooc_isInstanceOf( item, Foo ) );
	assertTrue( param == foreach_param );
	
	return FALSE;
}

static
void
test_foreach_until( void )
{
	Vector vector = vector_new_type( 10, Foo, OOC_MANAGE );
	int i;
	VectorIndex found;
	
	for(i=0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( 100 - i ) );
	assertTrue( vector_items( vector ) == 100 );

	found = vector_foreach_until_true( vector, (vector_item_checker) test_checker_always_false, &foreach_param );
	assertTrue( found == vector_items( vector ) );
	
	found = vector_foreach_until_true( vector, (vector_item_checker) item_is_17, &foreach_param );
	assertTrue( found >= 0 && found < 100 );
	assertTrue( found != 17 ); /* This must be the index of the item, not the value of it! */
	assertTrue( foo_get_data( ooc_cast( vector_get_item( vector, found ), Foo ) ) == 17 );
	
	ooc_delete( (Object) vector );	
}

static
int
can_divide( int num )
{
	return ( num % 7 == 0 ) ? TRUE : FALSE;
}

static
int
test_checker_dividable( Foo item, void * param )
{
	assertTrue( ooc_isInstanceOf( item, Foo ) );
	assertTrue( param == foreach_param );
	
	foreach_counter++;
	
	return can_divide( foo_get_data( item ) );
}

static
void
test_find_item( void )
{
	Vector vector = vector_new_type( 10, Foo, OOC_MANAGE );
	int i;
	VectorIndex found;
	
	for(i=0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( 100 - i ) );
	assertTrue( vector_items( vector ) == 100 );

	found = vector_find_item( vector, 0, (vector_item_checker) test_checker_always_false, &foreach_param );
	assertTrue( found == vector_items( vector ) );
	
	foreach_counter = 0;
	for( found = 0; found < vector_items( vector ); found ++) {
		found = vector_find_item( vector, found, (vector_item_checker) test_checker_dividable, &foreach_param );
		if( found < vector_items( vector ) )
			assertTrue( can_divide( foo_get_data( ooc_cast( vector_get_item( vector, found ), Foo ) ) ) );
		}
	assertTrue( foreach_counter == 100 );
	
	ooc_delete( (Object) vector );	
}

static
void
test_find_item_reverse( void )
{
	Vector vector = vector_new_type( 10, Foo, OOC_MANAGE );
	int i;
	VectorIndex found;
	
	for(i=0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( 100 - i ) );
	assertTrue( vector_items( vector ) == 100 );

	found = vector_find_item_reverse( vector, 99, (vector_item_checker) test_checker_always_false, &foreach_param );
	assertTrue( found == vector_items( vector ) );
	
	foreach_counter = 0;
	found = vector_items( vector );
	do {
		--found;
		found = vector_find_item_reverse( vector, found, (vector_item_checker) test_checker_dividable, &foreach_param );
		if( found != vector_items( vector ) )
			assertTrue( can_divide( foo_get_data( ooc_cast( vector_get_item( vector, found ), Foo ) ) ) );
		} while( found != vector_items( vector ) );
	assertTrue( foreach_counter == 100 );
	
	ooc_delete( (Object) vector );	
}

static
void
test_wrong_position_get_item( void )
{
	Vector vector = vector_new_type( 10, Foo, OOC_MANAGE );
	int i;
	
	try {
		vector_get_item( vector, 0 );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;
	
	for(i=0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( 100 - i ) );
	assertTrue( vector_items( vector ) == 100 );
	
	try {
		vector_get_item( vector, -1 );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	try {
		vector_get_item( vector, vector_items( vector ) );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	ooc_delete( (Object) vector );	
}

static
void
test_wrong_position_set_item( void )
{
	Vector vector = vector_new_type( 10, Foo, OOC_MANAGE );
	int i;
	
	try {
		vector_set_item( vector, 0, foo_new_with_data( 2 ) );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;
	
	for(i=0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( 100 - i ) );
	assertTrue( vector_items( vector ) == 100 );
	
	try {
		vector_set_item( vector, -1,  foo_new_with_data( 2 ) );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	try {
		vector_set_item( vector, vector_items( vector ), foo_new_with_data( 2 ) );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	ooc_delete( (Object) vector );	
}

static
void
test_wrong_position_insert( void )
{
	Vector vector = vector_new_type( 10, Foo, OOC_MANAGE );
	int i;
	
	try {
		vector_insert( vector, 1, foo_new_with_data( 2 ) );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;
	
	for(i=0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( 100 - i ) );
	assertTrue( vector_items( vector ) == 100 );
	
	try {
		vector_insert( vector, -1,  foo_new_with_data( 2 ) );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	try {
		vector_insert( vector, vector_items( vector )+1, foo_new_with_data( 2 ) );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	ooc_delete( (Object) vector );	
}

static
void
test_wrong_position_delete_item( void )
{
	Vector vector = vector_new_type( 10, Foo, OOC_MANAGE );
	int i;
	
	try {
		vector_delete_item( vector, 0 );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;
	
	for(i=0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( 100 - i ) );
	assertTrue( vector_items( vector ) == 100 );
	
	try {
		vector_delete_item( vector, -1 );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	try {
		vector_delete_item( vector, vector_items( vector ) );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	ooc_delete( (Object) vector );	
}

static
void
test_wrong_position_find_item( void )
{
	Vector vector = vector_new_type( 10, Foo, OOC_MANAGE );
	int i;
	
	try {
		vector_find_item( vector, 0, (vector_item_checker) test_checker_always_false, &foreach_param );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;
	
	for(i=0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( 100 - i ) );
	assertTrue( vector_items( vector ) == 100 );
	
	try {
		vector_find_item( vector, -1, (vector_item_checker) test_checker_always_false, &foreach_param );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	try {
		vector_find_item( vector, vector_items( vector ), (vector_item_checker) test_checker_always_false, &foreach_param );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	ooc_delete( (Object) vector );	
}

static
void
test_wrong_position_find_item_reverse( void )
{
	Vector vector = vector_new_type( 10, Foo, OOC_MANAGE );
	int i;
	
	try {
		vector_find_item_reverse( vector, 0, (vector_item_checker) test_checker_always_false, &foreach_param );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;
	
	for(i=0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( 100 - i ) );
	assertTrue( vector_items( vector ) == 100 );
	
	try {
		vector_find_item_reverse( vector, -1, (vector_item_checker) test_checker_always_false, &foreach_param );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	try {
		vector_find_item_reverse( vector, vector_items( vector ), (vector_item_checker) test_checker_always_false, &foreach_param );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	ooc_delete( (Object) vector );	
}

static
void
duplicate_untyped( VectorTest self )
{
	Vector volatile duplicate = NULL;
	Vector vector = vector_new( 10, (vector_item_destroyer) ooc_free );

	try {
		duplicate = (Vector) ooc_duplicate( (Object) vector );
		fail();
		}
	catch_any {
		assertTrue( exception_get_error_code( exception ) == err_can_not_be_duplicated );
		assertNull( duplicate );
		}
	end_try;

	ooc_delete( (Object) vector );
}

static
void
vector_check_sequential( Vector self )
{
	VectorIndex i;

	for( i=0; i<vector_items( self ); i++ )
	{
		Foo item = ooc_cast( vector_get_item( self, i ), Foo );
		assertTrue( foo_get_data( item ) == i );
	}
}
static
void
duplicate_typed( VectorTest self )
{
	Vector volatile duplicate = NULL;
	Vector vector = vector_new_type( 10, Foo, OOC_MANAGE );
	VectorIndex i;

	ooc_manage_object( vector );

	vector_push_back( vector, foo_new_with_data( 0 ) );
	vector_push_back( vector, foo_new_with_data( 1 ) );
	vector_push_back( vector, foo_new_with_data( 2 ) );
	vector_push_back( vector, foo_new_with_data( 3 ) );
	vector_push_back( vector, foo_new_with_data( 4 ) );

	duplicate = (Vector) ooc_duplicate( (Object) vector );

	assertTrue( vector_items(vector) == vector_items(duplicate) );
	for( i=0; i<vector_items(vector); i++ )
		assertTrue( vector_get_item( vector, i ) != vector_get_item( duplicate, i ) );

	ooc_delete( (Object) ooc_pass( vector ) );

	vector_check_sequential( duplicate );

	ooc_delete( (Object) duplicate );
}

/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */

ROM_ALLOC
struct TestCaseMethod methods[] =
{
	
	TEST(test_pushback),
	TEST(test_string_vector),
	TEST(test_untyped),
	TEST(test_type_safety_I),
	TEST(test_type_safety_II),
	TEST(test_type_safety_set_item),
	TEST(test_type_safety_insert),
	TEST(test_from_table),
	TEST(test_set_item),
	TEST(test_insert),
	TEST(test_delete),
	TEST(test_swap),
	TEST(test_foreach),
	TEST(test_foreach_until),
	TEST(test_find_item),
	TEST(test_find_item_reverse),
	TEST(test_wrong_position_get_item),
	TEST(test_wrong_position_set_item),
	TEST(test_wrong_position_insert),
	TEST(test_wrong_position_delete_item),
	TEST(test_wrong_position_find_item),
	TEST(test_wrong_position_find_item_reverse),
	TEST(duplicate_untyped),
	TEST(duplicate_typed),

	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
TESTCASE_MAIN
{
	VectorTest vectortest;
	int result;
	
	ooc_init_class( VectorTest );
	vectortest = (VectorTest) ooc_new( VectorTest, &methods );
	result = testcase_run((TestCase)vectortest);
	ooc_delete( (Object) vectortest );
	ooc_finalize_all();
	return result;
}
