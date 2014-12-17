
/* This is a ListTest class implementation file
 */

#include "../libs/exception.h"
#include "../libs/testcase.h"
#include "../libs/list.h"
#include "../libs/implement/list.h"

#include "foo.h"
#include "implement/foo.h"
#include "bar.h"
#include "implement/bar.h"

#ifdef OOC_NO_FINALIZE
#define ooc_finalize_class( x )
#define ooc_finalize_all( )
#endif

#define MAX_FOONODES 6		/* Maximal size of the FooNodePool */

/** @class ListTest
 *  @brief ListTest class - brief description.
 * 	@see listtest.h
 */

/** @file listtest.h 
 *  @brief ListTest class - brief description.
 * ListTest is a .....
 * @note	This class is a final class, can not be inherited.
 * @note	Run as: valgrind --leak-check=yes --quiet ./listtest
 */ 

DeclareClass( ListTest, TestCase );

Virtuals( ListTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( ListTest, TestCase )

	List	foonodelist;
	List 	foonodelist_um;
	
EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( ListTest, TestCase );


/* Class initializing
 */

static void listtest_before_class( ListTest self );
static void listtest_before( ListTest self );
static void listtest_after( ListTest self );
static void listtest_after_class( ListTest self );

static
void
ListTest_initialize( Class this )
{
	ListTestVtable vtab = (ListTestVtable) this->vtable;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) listtest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) listtest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) listtest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) listtest_after_class;

	ooc_init_class( Exception );
	
	ooc_init_class( List );

	ooc_init_class( Bar );
	ooc_init_class( Foo );

	/* Allocate global resources here */
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static
void
ListTest_finalize( Class this )
{
	/* Release global resources! */
}
#endif

/* Constructor
 */

static
void
ListTest_constructor( ListTest self, const void * params )
{
	assert( ooc_isInitialized( ListTest ) );
	
	chain_constructor( ListTest, self, params );
}

/* Destructor
 */

static
void
ListTest_destructor( ListTest self, ListTestVtable vtab )
{
}

/* Copy constuctor
 */

static
int
ListTest_copy( ListTest self, const ListTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/* This is a FooNode class implementation file
 */

DeclareClass( FooNode, ListNode );

ClassMembers( FooNode, ListNode )
	int		data;
EndOfClassMembers;

Virtuals( FooNode, ListNode )
EndOfVirtuals;

AllocateClass( FooNode, ListNode );

static	void	FooNode_initialize( Class this ) {}
#ifndef OOC_NO_FINALIZE
static	void	FooNode_finalize( Class this ) {}
#endif

static	void	FooNode_constructor( FooNode self, const void * params )
{
	assert( ooc_isInitialized( FooNode ) );
	
	chain_constructor( FooNode, self, NULL ); 
}

static	void	FooNode_destructor( FooNode self, FooNodeVtable vtab ) {}
static	int		FooNode_copy( FooNode self, const FooNode from ) { return OOC_COPY_DEFAULT; }

static
FooNode
foonode_new( void )
{
	static struct FooNodeObject		FooNodePool[ MAX_FOONODES ];
	static int 						foonodepool_counter = MAX_FOONODES-1;

	ooc_init_class( FooNode );

	if( ++foonodepool_counter >= MAX_FOONODES )
		foonodepool_counter = 0;

	if( FooNodePool[ foonodepool_counter ].ListNode.Base._vtab == FooNodeClass.vtable )
		ooc_throw( exception_new( err_out_of_memory ) );

	ooc_use( & FooNodePool[ foonodepool_counter ], FooNode, NULL );

	return & FooNodePool[ foonodepool_counter ];
}

static
FooNode
foonode_new_with_data( int data )
{
	FooNode self = foonode_new();
	self->data = data;
	return self;
}

static
int
foonode_get_data( FooNode self )
{
	return self->data;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
listtest_before_class( ListTest self )
{
	return;
}

static
void
listtest_before( ListTest self )
{
	static struct ListObject foonodelisto;
	static struct ListObject foonodelisto_um;

	self->foonodelist 		= & foonodelisto;
	self->foonodelist_um	= & foonodelisto_um;

	list_use_of_nodes( self->foonodelist, FooNode, OOC_MANAGE );
	list_use_of_nodes( self->foonodelist_um, FooNode, !OOC_MANAGE );
}

static
void
listtest_after( ListTest self )
{
	ooc_release( (Object) self->foonodelist );
	ooc_release( (Object) self->foonodelist_um );
}

static
void
listtest_after_class( ListTest self )
{
	return;
}


/*	=====================================================
	Test methods
 */

void
static
constructor( ListTest self )
{
	assertTrue( self->foonodelist->destroy == (list_item_destroyer) ooc_release );
	assertTrue( self->foonodelist->type == &FooNodeClass );
	assertTrue( self->foonodelist->list_of_nodes );
	assertNull( self->foonodelist->first );
	assertNull( self->foonodelist->last );

	assertNull( self->foonodelist_um->destroy );
	assertTrue( self->foonodelist_um->type == &FooNodeClass );
	assertTrue( self->foonodelist_um->list_of_nodes );
}

static
void
types( ListTest self )
{
	list_append( self->foonodelist, foonode_new() );
	assertTrue( ooc_isInstanceOf( self->foonodelist->first, FooNode ) );
	assertTrue( ooc_isInstanceOf( list_get_item( list_first( self->foonodelist )), FooNode ) );
}

static
void
type_safety( ListTest self )
{
	struct BarObject baro;

	ooc_use( & baro, Bar, NULL );

	try {
		list_append( self->foonodelist, &baro );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;

	try {
		list_prepend( self->foonodelist, &baro );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;

	try {
		list_insert_after( self->foonodelist, list_first(self->foonodelist), &baro );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;

	try {
		list_insert_before( self->foonodelist, list_last(self->foonodelist), &baro );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;

	try {
		struct ListObject listo;

		list_use_of_nodes( &listo, Foo, OOC_MANAGE );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;

}


static
void
iterator( ListTest self )
{
	list_append( self->foonodelist, foonode_new() );

	assertTrue( self->foonodelist->first == list_first( self->foonodelist ) );
	assertTrue( self->foonodelist->last == list_last( self->foonodelist ) );
	assertTrue( list_first( self->foonodelist ) == list_last( self->foonodelist ) );
	
	list_append( self->foonodelist, foonode_new() );
	
	assertTrue( self->foonodelist->first == list_first( self->foonodelist ) );
	assertTrue( self->foonodelist->last == list_last( self->foonodelist ) );
	assertFalse( list_first( self->foonodelist ) == list_last( self->foonodelist ) );
	assertTrue( list_last( self->foonodelist ) == list_next( self->foonodelist, list_first( self->foonodelist ) ) );
	assertTrue( list_first( self->foonodelist ) == list_previous( self->foonodelist, list_last( self->foonodelist ) ) );
}

static
void
check_sequential( List list )
{
	int i;
	ListIterator  iterator = list_first( list );
		
	for( i=0, iterator = list_first( list ); iterator != NULL; i++, iterator = list_next( list, iterator ) )
	{
		Object item = (Object) list_get_item( iterator );
		
		assertTrue( ooc_isInstanceOf( item, FooNode ) );
		assertTrue( foonode_get_data( ooc_cast( item, FooNode ) ) == i );
	}
}

static
void
append( ListTest self )
{
	int i;
	ListIterator    iterator;
	
	for( i=0; i<MAX_FOONODES; i++ )
		list_append( self->foonodelist, foonode_new_with_data( i ) );
		
	check_sequential( self->foonodelist );
}

static
void
paralell_append( ListTest self )
{
	ListIterator    iterator;
	int i;
	int check[ MAX_FOONODES ];
	
	for( i=0; i<MAX_FOONODES; i++ )
		check[ i ] = 0;

	#ifdef _OPENMP
	#pragma omp parallel for private(i)
	#endif
	for( i=0; i<MAX_FOONODES; i++ )
		list_append( self->foonodelist, foonode_new_with_data( i ) );

	for( i=0, iterator = list_first( self->foonodelist ); i<MAX_FOONODES; i++, iterator = list_next( self->foonodelist, iterator ) )
	{
		int index;
		index = foonode_get_data( ooc_cast( list_get_item( iterator ), FooNode ) );
		if( index >= 0 && index <= MAX_FOONODES )
			check[ index ] = index;
	}
	
	for( i=0; i<MAX_FOONODES; i++ )
		assertTrue( check[ i ] == i );
}

static
void
prepend( ListTest self )
{
	int i;
	ListIterator    iterator;
	
	for( i=0; i<MAX_FOONODES; i++ )
		list_prepend( self->foonodelist, foonode_new_with_data( MAX_FOONODES-1 - i ) );
		
	check_sequential( self->foonodelist );
}

static
void
paralell_prepend( ListTest self )
{
	ListIterator    iterator;
	int i;
	int check[ MAX_FOONODES ];
	
	for( i=0; i<MAX_FOONODES; i++ )
		check[ i ] = 0;

	#ifdef _OPENMP
	#pragma omp parallel for private(i)
	#endif
	for( i=0; i<MAX_FOONODES; i++ )
		list_prepend( self->foonodelist, foonode_new_with_data( i ) );

	for( i=0, iterator = list_first( self->foonodelist ); i<MAX_FOONODES; i++, iterator = list_next( self->foonodelist, iterator ) )
	{
		int index;
		index = foonode_get_data( ooc_cast( list_get_item( iterator ), FooNode ) );
		if( index >= 0 && index <= MAX_FOONODES )
			check[ index ] = index;
	}
	
	for( i=0; i<MAX_FOONODES; i++ )
		assertTrue( check[ i ] == i );
}

static
void
insert_before( ListTest self )
{
	ListIterator iterator;
	
	list_append( self->foonodelist, foonode_new_with_data( 1 ) );
	list_append( self->foonodelist, foonode_new_with_data( 3 ) );
	
	iterator = list_first( self->foonodelist );
	iterator = list_next( self->foonodelist, iterator );

	list_insert_before( self->foonodelist, iterator, foonode_new_with_data( 2 ) );

	iterator = list_first( self->foonodelist );

	list_insert_before( self->foonodelist, iterator, foonode_new_with_data( 0 ) );
	
	check_sequential( self->foonodelist );
}

static
void
insert_after( ListTest self )
{
	ListIterator iterator;
	
	list_append( self->foonodelist, foonode_new_with_data( 0 ) );
	list_append( self->foonodelist, foonode_new_with_data( 2 ) );
	
	iterator = list_first( self->foonodelist );

	list_insert_after( self->foonodelist, iterator, foonode_new_with_data( 1 ) );
	
	iterator = list_last( self->foonodelist );

	list_insert_after( self->foonodelist, iterator, foonode_new_with_data( 3 ) );
	
	check_sequential( self->foonodelist );
}

static
void
insert_first( ListTest self )
{
	list_insert_before( self->foonodelist, list_first(self->foonodelist), foonode_new_with_data( 1 ) );
	list_insert_before( self->foonodelist, list_first(self->foonodelist), foonode_new_with_data( 0 ) );

	check_sequential( self->foonodelist );
}

static
void
insert_last( ListTest self )
{
	list_insert_after( self->foonodelist, list_last(self->foonodelist), foonode_new_with_data( 0 ) );
	list_insert_after( self->foonodelist, list_last(self->foonodelist), foonode_new_with_data( 1 ) );

	check_sequential( self->foonodelist );
}

static
void
remove_item( ListTest self )
{
	ListIterator 	iterator;
	FooNode			foonode;
	
	list_append( self->foonodelist, foonode_new_with_data( 0 ) );
	list_append( self->foonodelist, foonode_new_with_data( 1 ) );
	list_append( self->foonodelist, foonode_new_with_data( 1 ) );
	list_append( self->foonodelist, foonode_new_with_data( 2 ) );
	
	iterator = list_first( self->foonodelist );
	iterator = list_next( self->foonodelist, iterator );
	iterator = list_next( self->foonodelist, iterator );

	foonode = list_remove_item( self->foonodelist, iterator );
	ooc_release( (Object) foonode );
		
	check_sequential( self->foonodelist );	
}

static
void
remove_first_item( ListTest self )
{
	FooNode		foonode;
	
	list_append( self->foonodelist, foonode_new_with_data( 34 ) );
	list_append( self->foonodelist, foonode_new_with_data( 0 ) );
	list_append( self->foonodelist, foonode_new_with_data( 1 ) );
	list_append( self->foonodelist, foonode_new_with_data( 2 ) );
	
	foonode = list_remove_first_item( self->foonodelist );
	ooc_release( (Object) foonode );
		
	check_sequential( self->foonodelist );
}

static
void
remove_last_item( ListTest self )
{
	FooNode		foonode;
	
	list_append( self->foonodelist, foonode_new_with_data( 0 ) );
	list_append( self->foonodelist, foonode_new_with_data( 1 ) );
	list_append( self->foonodelist, foonode_new_with_data( 2 ) );
	list_append( self->foonodelist, foonode_new_with_data( 34 ) );
	
	foonode = list_remove_last_item( self->foonodelist );
	ooc_release( (Object) foonode );
		
	check_sequential( self->foonodelist );	
}

void
remove_first_item_empty( ListTest self )
{
	assertNull( list_remove_first_item( self->foonodelist ) );
}

void
remove_last_item_empty( ListTest self )
{
	assertNull( list_remove_last_item( self->foonodelist ) );
}

static
void
remove_item_single( ListTest self )
{
	FooNode foonode;
	
	list_append( self->foonodelist, foonode_new_with_data( 0 ) );

	foonode = list_remove_item( self->foonodelist, list_first( self->foonodelist ) );
	
	assertNull( self->foonodelist->first );
	assertNull( self->foonodelist->last );
	
	ooc_release( (Object) foonode );
}

static
void
delete_item( ListTest self )
{
	ListIterator 	iterator;
	
	list_append( self->foonodelist, foonode_new_with_data( 0 ) );
	list_append( self->foonodelist, foonode_new_with_data( 1 ) );
	list_append( self->foonodelist, foonode_new_with_data( 1 ) );
	list_append( self->foonodelist, foonode_new_with_data( 2 ) );
	
	iterator = list_first( self->foonodelist );
	iterator = list_next( self->foonodelist, iterator );
	iterator = list_next( self->foonodelist, iterator );

	list_delete_item( self->foonodelist, iterator );
		
	check_sequential( self->foonodelist );	
}

static
void
delete_item_single( ListTest self )
{
	list_append( self->foonodelist, foonode_new_with_data( 0 ) );

	list_delete_item( self->foonodelist, list_first( self->foonodelist ) );
	
	assertNull( self->foonodelist->first );
	assertNull( self->foonodelist->last );
}

static
void
unmanaged( ListTest self )
{
	FooNode foonode1, foonode2;

	foonode1 = foonode_new();
	foonode2 = foonode_new();
	list_append( self->foonodelist_um, foonode1 );
	list_append( self->foonodelist_um, foonode2 );
	assertTrue( list_get_item( list_last( self->foonodelist_um ) ) == foonode2 );
	assertTrue( self->foonodelist_um->last == (void*) foonode2 );
	/* destructor must not leak */
	ooc_release( (Object) self->foonodelist_um );
	ooc_release( (Object) foonode1 );
	ooc_release( (Object) foonode2 );
}

static
void
swap_3( ListTest self )
{
	ListIterator i1, i2, i3, i4;
	
	i1 = list_append( self->foonodelist, foonode_new_with_data( 1 ) );
	i2 = list_append( self->foonodelist, foonode_new_with_data( 0 ) );
	i3 = list_append( self->foonodelist, foonode_new_with_data( 3 ) );
	i4 = list_append( self->foonodelist, foonode_new_with_data( 2 ) );
	
	list_swap( self->foonodelist, i1, i2 );
	list_swap( self->foonodelist, i3, i4 );

	check_sequential( self->foonodelist );
}

static
void
swap_4( ListTest self )
{
	ListIterator i1, i2, i3, i4;
	
	i1 = list_append( self->foonodelist, foonode_new_with_data( 3 ) );
	i2 = list_append( self->foonodelist, foonode_new_with_data( 2 ) );
	i3 = list_append( self->foonodelist, foonode_new_with_data( 1 ) );
	i4 = list_append( self->foonodelist, foonode_new_with_data( 0 ) );
	
	list_swap( self->foonodelist, i1, i4 );
	list_swap( self->foonodelist, i2, i3 );

	check_sequential( self->foonodelist );
}

static
void
swap_5( ListTest self )
{
	ListIterator i1, i2;

	i1 = list_append( self->foonodelist, foonode_new_with_data( 1 ) );
	i2 = list_append( self->foonodelist, foonode_new_with_data( 0 ) );

	list_swap( self->foonodelist, i1, i2 );

	check_sequential( self->foonodelist );
}

static ROM char *	foreach_param 		= "foreach parameter";
static int 			foreach_counter;

#define FOREACH_LOOP MAX_FOONODES

static
void
item_executor( void * item, void * param )
{
	assertTrue( param == (void*) foreach_param );
	assertTrue( ooc_isInstanceOf( item, FooNode ) );
	assertTrue( foonode_get_data( ooc_cast( item, FooNode ) ) == foreach_counter );
		
	foreach_counter++;
}

static
void
foreach( ListTest self )
{
	int i;
	
	/* check for an empty list */
	foreach_counter = 0;
	list_foreach( self->foonodelist, item_executor, (void*) foreach_param );
	assertTrue( foreach_counter == 0 );
	
	/* check with a ListNode data type */
	for( i=0; i<FOREACH_LOOP; i++ )
		list_append( self->foonodelist, foonode_new_with_data( i ) );
		
	foreach_counter = 0;
	list_foreach( self->foonodelist, item_executor, (void*) foreach_param );
	assertTrue( foreach_counter == FOREACH_LOOP );
}

static int expected_value;

static
int
item_checker( void * item, void * param )
{
	int current_value;
	
	assertTrue( param == (void*) foreach_param );
	assertTrue( ooc_isInstanceOf( item, FooNode ) );
	current_value = foonode_get_data( ooc_cast( item, FooNode ) );		
	assertTrue( current_value == foreach_counter );
	
	foreach_counter++;
	
	return current_value == expected_value;
}

static
int
foonode_comparator( void * item, void * param )
{
	assertTrue( param == (void*) foreach_param );
	assertTrue( ooc_get_type( item ) == &FooNodeClass );
	
	return foonode_get_data( ooc_cast( item, FooNode ) ) == expected_value;
}

static
void
foreach_until_true( ListTest self )
{
	int i;
	ListIterator found;
	
	/* check for an empty list */
	foreach_counter = 0;
	expected_value = FOREACH_LOOP/3;
	found = list_foreach_until_true( self->foonodelist, NULL, item_checker, (void*) foreach_param );
	assertTrue( foreach_counter == 0 );
	assertNull( found );
	
	/* check with a ListNode data type */
	for( i=0; i<FOREACH_LOOP; i++ )
		list_append( self->foonodelist, foonode_new_with_data( i ) );
		
	foreach_counter = 0;
	expected_value = FOREACH_LOOP/4;
	found = list_foreach_until_true( self->foonodelist, list_first( self->foonodelist ), item_checker, (void*) foreach_param );
	assertTrue( foreach_counter == FOREACH_LOOP/4 + 1 );
	assertTrue( foonode_get_data( list_get_item( found ) ) == FOREACH_LOOP/4 );

	expected_value = FOREACH_LOOP+1;
	foreach_counter = 0;
	found = list_foreach_until_true( self->foonodelist, list_first( self->foonodelist ), item_checker, (void*) foreach_param );
	assertTrue( foreach_counter == FOREACH_LOOP );
	assertNull( found );

	expected_value = FOREACH_LOOP/4;
	foreach_counter = FOREACH_LOOP-1;
	found = list_foreach_until_true( self->foonodelist, list_last( self->foonodelist ), item_checker, (void*) foreach_param );
	assertTrue( foreach_counter == FOREACH_LOOP );
	assertNull( found );
}

static
void
foreach_delete_if( ListTest self )
{
	int i;
	
	expected_value = FOREACH_LOOP;
	
	for( i=0; i<FOREACH_LOOP-2; i++ )
	{
		list_append( self->foonodelist, foonode_new_with_data( i ) );
		if( i % 2 != 0 )
			list_append( self->foonodelist, foonode_new_with_data( expected_value ) );
	}
	
	list_foreach_delete_if( self->foonodelist, foonode_comparator, (void*) foreach_param );
	check_sequential( self->foonodelist );
}

static
void
find_item( ListTest self )
{
	int i, to_be_found, found_counter;
	ListIterator found;
	
	expected_value = FOREACH_LOOP+1;
	
	for( i=0, to_be_found= 0; i<FOREACH_LOOP-2; i++ )
	{
		list_append( self->foonodelist, foonode_new_with_data( i ) );
		if( i % 2 != 0 ) {
			list_append( self->foonodelist, foonode_new_with_data( expected_value ) );
			to_be_found++;
		}
	}
	assertNotZero( to_be_found );

	found_counter = 0;
	found = NULL;
	do {
		found = list_find_item( self->foonodelist, found, foonode_comparator, (void*) foreach_param );
		if( found )
		{
			found_counter++;
			assertTrue( foonode_get_data( ooc_cast( list_get_item( found ), FooNode ) ) == expected_value );
			found = list_next( self->foonodelist, found );
		}
	} while( found );
	assertTrue( found_counter == to_be_found );
}

static
void
find_item_reverse( ListTest self )
{
	int i, to_be_found, found_counter;
	ListIterator found;
	
	expected_value = FOREACH_LOOP+1;
	
	for( i=0, to_be_found= 0; i<FOREACH_LOOP-2; i++ )
	{
		list_append( self->foonodelist, foonode_new_with_data( i ) );
		if( i % 2 != 0 ) {
			list_append( self->foonodelist, foonode_new_with_data( expected_value ) );
			to_be_found++;
		}
	}
	assertNotZero( to_be_found );
	
	found_counter = 0;
	found = NULL;
	do {
		found = list_find_item_reverse( self->foonodelist, found, foonode_comparator, (void*) foreach_param );
		if( found )
		{
			found_counter++;
			assertTrue( foonode_get_data( ooc_cast( list_get_item( found ), FooNode ) ) == expected_value );
			found = list_previous( self->foonodelist, found );
		}
	} while( found );
	assertTrue( found_counter == to_be_found );
}

static
void
bad_reuse( ListTest self )
{
	FooNode	node;
	ListIterator position;
	
	list_append( self->foonodelist, foonode_new_with_data( 0 ) );
	list_append( self->foonodelist, foonode_new_with_data( 1 ) );
	
	node = foonode_new_with_data( 2 );
	list_append( self->foonodelist, node );

	position = list_append( self->foonodelist, foonode_new_with_data( 3 ) );
	
	list_append( self->foonodelist, foonode_new_with_data( 4 ) );
	list_append( self->foonodelist, foonode_new_with_data( 5 ) );
	
	try {
		list_append( self->foonodelist, node );
		fail();
		}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_already_in_use );
	end_try;
	
	try {
		list_prepend( self->foonodelist, node );
		fail();
		}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_already_in_use );
	end_try;
	
	try {
		list_insert_after( self->foonodelist, position, node );
		fail();
		}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_already_in_use );
	end_try;
	
	try {
		list_insert_before( self->foonodelist, position, node );
		fail();
		}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_already_in_use );
	end_try;
}

/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
ROM
struct TestCaseMethod methods[] =
{
	TEST(constructor),
	TEST(types),
	TEST(type_safety),
	TEST(iterator),
	TEST(append),
	TEST(paralell_append),
	TEST(prepend),
	TEST(paralell_prepend),
	TEST(insert_before),
	TEST(insert_after),
	TEST(insert_first),
	TEST(insert_last),
	TEST(remove_item),
	TEST(remove_item_single),
	TEST(remove_first_item),
	TEST(remove_last_item),
	TEST(remove_first_item_empty),
	TEST(remove_last_item_empty),
	TEST(delete_item),
	TEST(delete_item_single),
	TEST(unmanaged),
	TEST(swap_3),
	TEST(swap_4),
	TEST(swap_5),
	TEST(foreach),
	TEST(foreach_until_true),
	TEST(foreach_delete_if),
	TEST(find_item),
	TEST(find_item_reverse),
	TEST(bad_reuse),

	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
typedef struct TestCaseObject * TestCase;

TESTCASE_MAIN
{
	struct ListTestObject listtesto;
	int result;
	
	ooc_init_class( ListTest );
	ooc_use( &listtesto, ListTest, (void*) &methods );
	result = testcase_run((TestCase) & listtesto );
	ooc_release( (Object) & listtesto );
	ooc_finalize_all();

	abort();
}
