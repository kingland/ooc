
/* This is a VectorTest class implementation file
 */

#include "../libs/testcase.h"

/* Testable */
#include "../libs/vector.h"
#include "../libs/implement/vector.h"

#include "foo.h"
#include "implement/foo.h"
#include "bar.h"
#include "implement/bar.h"
#include "barson.h"
#include "implement/barson.h"
#include "bardaughter.h"
#include "implement/bardaughter.h"

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
	VectorTestVtable vtab = (VectorTestVtable) this->vtable;
	
	vtab->TestCase.before_class	= 	(test_method_type) vectortest_before_class;
	vtab->TestCase.before		= 	(test_method_type) vectortest_before;
	vtab->TestCase.after		= 	(test_method_type) vectortest_after;
	vtab->TestCase.after_class	= 	(test_method_type) vectortest_after_class;

	ooc_init_class( Exception );
	ooc_init_class( Vector );
	/* Allocate global resources here */
}

/* Class finalizing
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

#define VECTOR_SIZE	10

void * 	vectorStore[ VECTOR_SIZE ];

static struct FooObject	foo_array[ VECTOR_SIZE ];
static struct FooObject	foo_array_2[ VECTOR_SIZE ];


static struct FooObject	foo_o;
static struct BarObject bar_o;
static struct BarSonObject barSon_o;
static struct BarDaughterObject barDaughter_o;

/*	=====================================================
	Test methods
 */

static
void
test_pushback( void )
{
	struct VectorObject	vector_o;
	VectorIndex i;

	vector_use_type_with_store( & vector_o, VECTOR_SIZE, Foo, TRUE, vectorStore );
	
	#ifdef _OPENMP
	#pragma omp parallel for private(i) schedule( guided, 3 )
	#endif
	for( i = 0; i<VECTOR_SIZE; i++ ) {
		foo_use_with_data( &foo_array[i], VECTOR_SIZE - i );
		vector_push_back( &vector_o, &foo_array[i] );
		}	
		
	assertTrue( vector_items( &vector_o ) == VECTOR_SIZE );

	for( i = 0; i<VECTOR_SIZE; i++ ) {
		int value = foo_get_data( vector_get_item( &vector_o, i ) );
		assertTrue( value == VECTOR_SIZE - i );
		}	

	ooc_release( (Object) &vector_o );
}

static
void
test_untyped( void )
{
	struct VectorObject vector_o;

	vector_use_with_store( &vector_o, VECTOR_SIZE, (vector_item_destroyer) ooc_release, vectorStore );

	bar_use_with_data( &bar_o, 1 );
	foo_use_with_data( &foo_o, 2 );
	barson_use_with_data( &barSon_o, 3 );
	bardaughter_use_with_data( &barDaughter_o, 4 );

	/* Must NOT violate type safety */

	vector_push_back( &vector_o, &foo_o );
	vector_push_back( &vector_o, &bar_o );
	vector_push_back( &vector_o, &barSon_o );
	vector_push_back( &vector_o, &barDaughter_o );
		
	ooc_release( (Object) &vector_o );
}

static
void
test_type_safety_I( void )
{
	struct VectorObject vector_o;

	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Foo, TRUE, vectorStore );

	/* Must violate type safety */
	
	try {
		bar_use_with_data( &bar_o, 1 );
		vector_push_back( &vector_o, &bar_o );
		fail();
		}
	catch_any {
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	}
	end_try;
	
	ooc_release( (Object) &vector_o );
}

static
void
test_type_safety_II( void )
{
	struct VectorObject vector_o;

	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Bar, TRUE, vectorStore );

	bar_use_with_data( &bar_o, 1 );
	barson_use_with_data( &barSon_o, 2 );
	bardaughter_use_with_data( &barDaughter_o, 3 );

	/* Must NOT violate type safety */

	vector_push_back( &vector_o, &bar_o );
	vector_push_back( &vector_o, &barSon_o );
	vector_push_back( &vector_o, &barDaughter_o );
		
	ooc_release( (Object) &vector_o );
}


static
void
test_type_safety_set_item( void )
{
	struct VectorObject vector_o;
	
	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Bar, TRUE, vectorStore );

	bar_use_with_data( &bar_o, 1 );
	foo_use_with_data( &foo_o, 2 );

	vector_push_back( &vector_o, &bar_o );
	try {
		vector_set_item( &vector_o, 0, &foo_o );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;

	ooc_release( (Object) &vector_o );
	
	vector_use_with_store( &vector_o, VECTOR_SIZE, (vector_item_destroyer) ooc_release, vectorStore );

	bar_use_with_data( &bar_o, 1 );
	foo_use_with_data( &foo_o, 2 );
	barson_use_with_data( &barSon_o, 3 );

	vector_push_back( &vector_o, &bar_o );
	vector_set_item( &vector_o, 0, &foo_o );
	vector_set_item( &vector_o, 0, &barSon_o );

	ooc_release( (Object) &vector_o );
}

static
void
test_type_safety_insert( void )
{
	struct VectorObject vector_o;
	
	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Bar, TRUE, vectorStore );

	bar_use_with_data( &bar_o, 1 );
	foo_use_with_data( &foo_o, 2 );

	vector_push_back( &vector_o, &bar_o );
	try {
		vector_insert( &vector_o, 0, &foo_o );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;

	ooc_release( (Object) &vector_o );
	
	vector_use_with_store( &vector_o, VECTOR_SIZE, (vector_item_destroyer) ooc_release, vectorStore );

	bar_use_with_data( &bar_o, 1 );
	foo_use_with_data( &foo_o, 2 );
	barson_use_with_data( &barSon_o, 3 );

	vector_push_back( &vector_o, &bar_o );
	vector_insert( &vector_o, 0, &foo_o );
	vector_insert( &vector_o, 0, &barSon_o );

	ooc_release( (Object) &vector_o );
}

static
void
test_set_item( void )
{
	struct VectorObject vector_o;
	int i;

	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Foo, TRUE, vectorStore );

	for( i = 0; i<VECTOR_SIZE; i++ ) {
		foo_use_with_data( &foo_array_2[i], i );
		vector_push_back( &vector_o, &foo_array_2[i] );
		}	

	assertTrue( vector_items( &vector_o ) == VECTOR_SIZE );
	
	for( i = 0; i<VECTOR_SIZE; i++ )
		foo_use_with_data( &foo_array[i], VECTOR_SIZE-i );
		
	#ifdef _OPENMP
	#pragma omp parallel for private(i) schedule( guided, 3 )
	#endif
	for( i = 0; i<VECTOR_SIZE; i++ )
		vector_set_item( &vector_o, i, &foo_array[i] );
		
	for( i = 0; i<VECTOR_SIZE; i++ )
		assertTrue( foo_get_data( ooc_cast( vector_get_item( &vector_o, i ), Foo ) ) ==  VECTOR_SIZE - i );
		
	ooc_release( (Object) &vector_o );
}

static
void
test_insert( void )
{
	struct VectorObject vector_o;
	int i;

	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Foo, TRUE, vectorStore );
	
	for( i = 0; i<VECTOR_SIZE; i++ )
		foo_use_with_data( &foo_array[i], i );

	for(i=0; i<VECTOR_SIZE; i++ )
		if( i % 3 != 0 )
			vector_push_back( &vector_o, &foo_array[i] );

	for(i=0; i<VECTOR_SIZE; i++ )
		if( i % 3 == 0 )
			vector_insert( &vector_o, i, &foo_array[i] );

	for(i=0; i<VECTOR_SIZE; i++ ) {
		Foo actual = ooc_cast( vector_get_item( &vector_o, i ), Foo );
		assertTrue( foo_get_data( actual ) == i );
	}
	
	ooc_release( (Object) &vector_o );	
}

static
void
test_delete( void )
{
	struct VectorObject vector_o;
	VectorIndex i, j;

	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Foo, TRUE, vectorStore );
	
	for( i = 0; i<VECTOR_SIZE; i++ ) {
		foo_use_with_data( &foo_array[i], i );
		vector_push_back( &vector_o, &foo_array[i] );
		}	

	assertTrue( vector_items( &vector_o ) == VECTOR_SIZE );

	for(i=0; i < vector_items( &vector_o ); )
		if( i % 3 == 0 )
			vector_delete_item( &vector_o, i );
		else
			i++;
	assertTrue( vector_items( &vector_o ) < VECTOR_SIZE );

	for(i=0, j=0; i < vector_items( &vector_o ); i++, j++ ) {
		Foo actual;
		
		if( j % 3 == 0 )
			j++;
			
		actual = ooc_cast( vector_get_item( &vector_o, i ), Foo );
		assertTrue( foo_get_data( actual ) == j );
		}
				
	ooc_release( (Object) &vector_o );
}

static
void
test_swap( void )
{
	struct VectorObject vector_o;
	int i;

	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Foo, TRUE, vectorStore );
	
	for( i = 0; i<VECTOR_SIZE; i++ )
		foo_use_with_data( &foo_array[i], i );

	for(i=0; i<VECTOR_SIZE/2; i++ ) {
		vector_push_back( &vector_o, &foo_array[ 2*i+1 ] );
		vector_push_back( &vector_o, &foo_array[ 2*i ] );
		}
	assertTrue( vector_items( &vector_o ) == (VECTOR_SIZE/2)*2 );

	for(i=0; i<VECTOR_SIZE/2; i++ )
		vector_swap( &vector_o, 2*i, 2*i+1 );
	assertTrue( vector_items( &vector_o ) == (VECTOR_SIZE/2)*2 );
	
	for(i=0; i<VECTOR_SIZE; i++ ) {
		Foo actual = ooc_cast( vector_get_item( &vector_o, i ), Foo );
		assertTrue( foo_get_data( actual ) == i );
		}
	
	ooc_release( (Object) &vector_o );	
}

static const char foreach_param[] = "parameter";
static int foreach_counter;

static
void
test_item_executor( Foo item, void * param )
{
	static int previous;
	
	assertTrue( ooc_isInstanceOf( item, Foo ) );
	assertTrue( param == foreach_param );
	
	assertTrue( foo_get_data( item ) == foreach_counter );
	foreach_counter++;
}

static
void
test_foreach( void )
{
	struct VectorObject vector_o;
	VectorIndex i;

	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Foo, TRUE, vectorStore );
	
	for( i = 0; i<VECTOR_SIZE; i++ ) {
		foo_use_with_data( &foo_array[i], i );
		vector_push_back( &vector_o, &foo_array[i] );
		}	

	assertTrue( vector_items( &vector_o ) == VECTOR_SIZE );

	foreach_counter = 0;
	vector_foreach( &vector_o, (vector_item_executor) test_item_executor, (void*) &foreach_param );
	assertTrue( foreach_counter == vector_items( &vector_o ) );
	
	ooc_release( (Object) &vector_o );	
}

static
int
item_is_7( Foo item, void * param )
{
	assertTrue( ooc_isInstanceOf( item, Foo ) );
	assertTrue( param == foreach_param );
	
	return ( foo_get_data( item ) == 7 ) ? TRUE : FALSE;
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
	struct VectorObject vector_o;
	VectorIndex i, found;

	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Foo, TRUE, vectorStore );
	
	for( i = 0; i<VECTOR_SIZE; i++ ) {
		foo_use_with_data( &foo_array[i], VECTOR_SIZE-i );
		vector_push_back( &vector_o, &foo_array[i] );
		}	

	assertTrue( vector_items( &vector_o ) == VECTOR_SIZE );

	found = vector_foreach_until_true( &vector_o, (vector_item_checker) test_checker_always_false, (void*) &foreach_param );
	assertTrue( found == vector_items( &vector_o ) );
	
	found = vector_foreach_until_true( &vector_o, (vector_item_checker) item_is_7, (void*) &foreach_param );
	assertTrue( found >= 0 && found < VECTOR_SIZE );
	assertTrue( found != 7 ); /* This must be the index of the item, not the value of it! */
	assertTrue( foo_get_data( ooc_cast( vector_get_item( &vector_o, found ), Foo ) ) == 7 );
	
	ooc_release( (Object) &vector_o );	
}

static
int
can_divide( int num )
{
	return ( num % 3 == 0 ) ? TRUE : FALSE;
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
	struct VectorObject vector_o;
	VectorIndex i, found;
	int found_counter = 0;

	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Foo, TRUE, vectorStore );
	
	for( i = 0; i<VECTOR_SIZE; i++ ) {
		foo_use_with_data( &foo_array[i], VECTOR_SIZE-i );
		vector_push_back( &vector_o, &foo_array[i] );
		}	

	assertTrue( vector_items( &vector_o ) == VECTOR_SIZE );

	found = vector_find_item( &vector_o, 0, (vector_item_checker) test_checker_always_false, (void*) &foreach_param );
	assertTrue( found == vector_items( &vector_o ) );
	
	foreach_counter = 0;
	for( found = 0; found < vector_items( &vector_o ); found ++) {
		found = vector_find_item( &vector_o, found, (vector_item_checker) test_checker_dividable, (void*)&foreach_param );
		if( found < vector_items( &vector_o ) ) {
			found_counter++;
			assertTrue( can_divide( foo_get_data( ooc_cast( vector_get_item( &vector_o, found ), Foo ) ) ) );
			}
		}
	assertNotZero( found_counter );
	assertTrue( foreach_counter == VECTOR_SIZE );
	
	ooc_release( (Object) &vector_o );	
}

static
void
test_find_item_reverse( void )
{
	struct VectorObject vector_o;
	VectorIndex i, found;
	int found_counter = 0;

	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Foo, TRUE, vectorStore );
	
	for( i = 0; i<VECTOR_SIZE; i++ ) {
		foo_use_with_data( &foo_array[i], VECTOR_SIZE-i );
		vector_push_back( &vector_o, &foo_array[i] );
		}	

	assertTrue( vector_items( &vector_o ) == VECTOR_SIZE );

	found = vector_find_item_reverse( &vector_o, VECTOR_SIZE-1, (vector_item_checker) test_checker_always_false, (void*)&foreach_param );
	assertTrue( found == vector_items( &vector_o ) );
	
	foreach_counter = 0;
	found = vector_items( &vector_o );
	do {
		--found;
		found = vector_find_item_reverse( &vector_o, found, (vector_item_checker) test_checker_dividable, (void*)&foreach_param );
		if( found != vector_items( &vector_o ) ) {
			found_counter++;
			assertTrue( can_divide( foo_get_data( ooc_cast( vector_get_item( &vector_o, found ), Foo ) ) ) );
			}
		} while( found != vector_items( &vector_o ) );
	assertNotZero( found_counter );
	assertTrue( foreach_counter == VECTOR_SIZE );
	
	ooc_release( (Object) &vector_o );	
}

static
void
test_wrong_position_get_item( void )
{
	struct VectorObject vector_o;
	VectorIndex i;

	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Foo, TRUE, vectorStore );
	
	try {
		vector_get_item( &vector_o, 0 );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;
	
	for( i = 0; i<VECTOR_SIZE; i++ ) {
		foo_use_with_data( &foo_array[i], VECTOR_SIZE-i );
		vector_push_back( &vector_o, &foo_array[i] );
		}	
	assertTrue( vector_items( &vector_o ) == VECTOR_SIZE );
	
	try {
		vector_get_item( &vector_o, -1 );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	try {
		vector_get_item( &vector_o, vector_items( &vector_o ) );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	ooc_release( (Object) &vector_o );	
}

static
void
test_wrong_position_set_item( void )
{
	struct VectorObject vector_o;
	VectorIndex i, found;
	int found_counter = 0;

	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Foo, TRUE, vectorStore );

	try {
		foo_use_with_data( &foo_o, 77 );
		vector_set_item( &vector_o, 0, &foo_o );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;
	
	for( i = 0; i<VECTOR_SIZE; i++ ) {
		foo_use_with_data( &foo_array[i], VECTOR_SIZE-i );
		vector_push_back( &vector_o, &foo_array[i] );
		}	
	assertTrue( vector_items( &vector_o ) == VECTOR_SIZE );
	
	try {
		foo_use_with_data( &foo_o, 77 );
		vector_set_item( &vector_o, -1,  &foo_o );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	try {
		foo_use_with_data( &foo_o, 77 );
		vector_set_item( &vector_o, vector_items( &vector_o ), &foo_o );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	ooc_release( (Object) &vector_o );	
}

static
void
test_wrong_position_insert( void )
{
	struct VectorObject vector_o;
	VectorIndex i;

	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Foo, TRUE, vectorStore );
	
	try {
		foo_use_with_data( &foo_o, 77 );
		vector_insert( &vector_o, 1, &foo_o );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;
	
	for( i = 0; i<VECTOR_SIZE; i++ ) {
		foo_use_with_data( &foo_array[i], VECTOR_SIZE-i );
		vector_push_back( &vector_o, &foo_array[i] );
		}	
	assertTrue( vector_items( &vector_o ) == VECTOR_SIZE );
	
	try {
		foo_use_with_data( &foo_o, 77 );
		vector_insert( &vector_o, -1,  &foo_o );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	try {
		foo_use_with_data( &foo_o, 77 );
		vector_insert( &vector_o, vector_items( &vector_o )+1, &foo_o );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	ooc_release( (Object) &vector_o );	
}

static
void
test_wrong_position_delete_item( void )
{
	struct VectorObject vector_o;
	VectorIndex i;

	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Foo, TRUE, vectorStore );

	try {
		vector_delete_item( &vector_o, 0 );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;
	
	for( i = 0; i<VECTOR_SIZE; i++ ) {
		foo_use_with_data( &foo_array[i], VECTOR_SIZE-i );
		vector_push_back( &vector_o, &foo_array[i] );
		}	
	assertTrue( vector_items( &vector_o ) == VECTOR_SIZE );
	
	try {
		vector_delete_item( &vector_o, -1 );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	try {
		vector_delete_item( &vector_o, vector_items( &vector_o ) );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	ooc_release( (Object) &vector_o );	
}

static
void
test_wrong_position_find_item( void )
{
	struct VectorObject vector_o;
	VectorIndex i;

	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Foo, TRUE, vectorStore );
	
	try {
		vector_find_item( &vector_o, 0, (vector_item_checker) test_checker_always_false, (void*) &foreach_param );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;
	
	for( i = 0; i<VECTOR_SIZE; i++ ) {
		foo_use_with_data( &foo_array[i], VECTOR_SIZE-i );
		vector_push_back( &vector_o, &foo_array[i] );
		}	
	assertTrue( vector_items( &vector_o ) == VECTOR_SIZE );
	
	try {
		vector_find_item( &vector_o, -1, (vector_item_checker) test_checker_always_false, (void*) &foreach_param );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	try {
		vector_find_item( &vector_o, vector_items( &vector_o ), (vector_item_checker) test_checker_always_false, (void*) &foreach_param );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	ooc_release( (Object) &vector_o );	
}

static
void
test_wrong_position_find_item_reverse( void )
{
	struct VectorObject vector_o;
	VectorIndex i;

	vector_use_type_with_store( &vector_o, VECTOR_SIZE, Foo, TRUE, vectorStore );
	
	try {
		vector_find_item_reverse( &vector_o, 0, (vector_item_checker) test_checker_always_false, (void*)&foreach_param );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;
	
	for( i = 0; i<VECTOR_SIZE; i++ ) {
		foo_use_with_data( &foo_array[i], VECTOR_SIZE-i );
		vector_push_back( &vector_o, &foo_array[i] );
		}	
	assertTrue( vector_items( &vector_o ) == VECTOR_SIZE );
	
	try {
		vector_find_item_reverse( &vector_o, -1, (vector_item_checker) test_checker_always_false, (void*)&foreach_param );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	try {
		vector_find_item_reverse( &vector_o, vector_items( &vector_o ), (vector_item_checker) test_checker_always_false, (void*)&foreach_param );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	ooc_release( (Object) &vector_o );	
}

/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
ROM struct TestCaseMethod methods[] =
{
	
	TEST(test_pushback),
	TEST(test_untyped),
	TEST(test_type_safety_I),
	TEST(test_type_safety_II),
	TEST(test_type_safety_set_item),
	TEST(test_type_safety_insert),
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

	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */

typedef struct TestCaseObject * TestCase;

TESTCASE_MAIN
{
	struct VectorTestObject vectortesto;
	int result;
	
	ooc_init_class( VectorTest );
	ooc_use( &vectortesto, VectorTest, (void*) &methods );
	result = testcase_run( (TestCase) &vectortesto );
	ooc_release( (Object) & vectortesto );
	ooc_finalize_all();

	abort();
}
