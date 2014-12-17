
/* This is a ListTest class implementation file
 */

#include "../libs/exception.h"
#include "../libs/testcase.h"
#include "../libs/list.h"
#include "../libs/implement/list.h"

#include "foo.h"
#include "bar.h"

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

#ifdef OOC_NO_FINALIZE
#define ooc_finalize_class( x )
#define ooc_finalize_all( )
#endif

DeclareClass( ListTest, TestCase );

Virtuals( ListTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( ListTest, TestCase )

	List	list;
	List	foolist;
	List	foonodelist;
	
	List	list_um;
	List	foolist_um;
	List	foonodelist_um;
	
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
	ListTestVtable vtab = & ListTestVtableInstance;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) listtest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) listtest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) listtest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) listtest_after_class;

	ooc_init_class( Exception );
	
	ooc_init_class( List );

	ooc_init_class( Foo );
	ooc_init_class( Bar );

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
foonode_new( )
{
	ooc_init_class( FooNode );
	return ooc_new( FooNode, NULL );
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
	self->list = list_new( ooc_free );
	self->foolist = list_new_type( Foo, OOC_MANAGE );
	self->foonodelist = list_new_of_nodes( FooNode, OOC_MANAGE );

	self->list_um = list_new( NULL );
	self->foolist_um = list_new_type( Foo, !OOC_MANAGE );
	self->foonodelist_um = list_new_of_nodes( FooNode, !OOC_MANAGE );
}

static
void
listtest_after( ListTest self )
{
	ooc_delete_and_null( (Object*) &self->list );
	ooc_delete_and_null( (Object*) &self->foolist );
	ooc_delete_and_null( (Object*) &self->foonodelist );

	ooc_delete_and_null( (Object*) &self->list_um );
	ooc_delete_and_null( (Object*) &self->foolist_um );
	ooc_delete_and_null( (Object*) &self->foonodelist_um );
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
	assertTrue( self->list->destroy == ooc_free );
	assertTrue( self->list->type == NULL );
	assertFalse( self->list->list_of_nodes );
	assertNull( self->list->first );
	assertNull( self->list->last );
	assertTrue( list_get_managed( self->list ) == OOC_MANAGE );

	assertTrue( self->foolist->destroy == (list_item_destroyer) ooc_delete );
	assertTrue( self->foolist->type == &FooClass );
	assertFalse( self->foolist->list_of_nodes );
	assertNull( self->foolist->first );
	assertNull( self->foolist->last );
	assertTrue( list_get_managed( self->foolist ) == OOC_MANAGE );

	assertTrue( self->foonodelist->destroy == (list_item_destroyer) ooc_delete );
	assertTrue( self->foonodelist->type == &FooNodeClass );
	assertTrue( self->foonodelist->list_of_nodes );
	assertNull( self->foonodelist->first );
	assertNull( self->foonodelist->last );
	assertTrue( list_get_managed( self->foonodelist ) == OOC_MANAGE );

	assertNull( self->list_um->destroy );
	assertTrue( self->list_um->type == NULL );
	assertFalse( self->list_um->list_of_nodes );
	assertTrue( list_get_managed( self->list_um ) == !OOC_MANAGE );

	assertNull( self->foolist_um->destroy );
	assertTrue( self->foolist_um->type == &FooClass );
	assertFalse( self->foolist_um->list_of_nodes );
	assertTrue( list_get_managed( self->foolist_um ) == !OOC_MANAGE );

	assertNull( self->foonodelist_um->destroy );
	assertTrue( self->foonodelist_um->type == &FooNodeClass );
	assertTrue( self->foonodelist_um->list_of_nodes );
	assertTrue( list_get_managed( self->foonodelist_um ) == !OOC_MANAGE );
}

static
void
types( ListTest self )
{
	list_append( self->list, ooc_malloc( 100 ) );
	assertTrue( ooc_isInstanceOf( self->list->first, ListNode ) );
	assertNull( list_get_type( self->list ) );
	
	list_append( self->foolist, foo_new() );
	assertTrue( ooc_isInstanceOf( self->foolist->first, ListNode ) );
	assertTrue( ooc_isInstanceOf( list_get_item( list_first( self->foolist )), Foo ) );
	assertTrue( list_get_type( self->foolist ) == & FooClass );
	
	list_append( self->foonodelist, foonode_new() );
	assertTrue( ooc_isInstanceOf( self->foonodelist->first, FooNode ) );
	assertTrue( ooc_isInstanceOf( list_get_item( list_first( self->foonodelist )), FooNode ) );
	assertTrue( list_get_type( self->foonodelist ) == & FooNodeClass );
}

static
void
type_safety( ListTest self )
{
	try {
		list_append( self->foolist, bar_new() );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;

	try {
		list_prepend( self->foolist, bar_new() );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;

	try {
		list_insert_after( self->foolist, list_first(self->foolist), bar_new() );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;

	try {
		list_insert_before( self->foolist, list_last(self->foolist), bar_new() );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;

	try {
		list_new_of_nodes( Foo, OOC_MANAGE );
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
	list_append( self->foolist, foo_new() );

	assertTrue( self->foolist->first == list_first( self->foolist ) );
	assertTrue( self->foolist->last == list_last( self->foolist ) );
	assertTrue( list_first( self->foolist ) == list_last( self->foolist ) );
	
	list_append( self->foolist, foo_new() );
	
	assertTrue( self->foolist->first == list_first( self->foolist ) );
	assertTrue( self->foolist->last == list_last( self->foolist ) );
	assertFalse( list_first( self->foolist ) == list_last( self->foolist ) );
	assertTrue( list_last( self->foolist ) == list_next( self->foolist, list_first( self->foolist ) ) );
	assertTrue( list_first( self->foolist ) == list_previous( self->foolist, list_last( self->foolist ) ) );
	
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
		
		if( ooc_isInstanceOf( item, Foo ) ) {
			assertTrue( foo_get_data( ooc_cast( item, Foo ) ) == i );
			}
		else if( ooc_isInstanceOf( item, FooNode ) ) {
			assertTrue( foonode_get_data( ooc_cast( item, FooNode ) ) == i );
			}	
		else
			failMsg("Unknown item type in list");
	}
}

static
void
append( ListTest self )
{
	static const int list_size = 100;
	int i;
	ListIterator    iterator;
	
	for( i=0; i<list_size; i++ )
		list_append( self->foolist, foo_new_with_data( i ) );
		
	for( i=0, iterator = list_first( self->foolist ); i<list_size; i++, iterator = list_next( self->foolist, iterator ) )
		assertTrue( foo_get_data( ooc_cast( list_get_item( iterator ), Foo ) ) == i );
}

static
void
paralell_append( ListTest self )
{
	static const int check_size = 100;
	ListIterator    iterator;
	int i;
	int * check = ooc_calloc( check_size, sizeof( int ) );
	
	ooc_manage( check, ooc_free );
	
	for( i=0; i<check_size; i++ )
		check[ i ] = 0;

	#ifdef _OPENMP
	#pragma omp parallel for private(i)
	#endif
	for( i=0; i<check_size; i++ )
		list_append( self->foolist, foo_new_with_data( i ) );

	for( i=0, iterator = list_first( self->foolist ); i<check_size; i++, iterator = list_next( self->foolist, iterator ) )
	{
		int index;
		index = foo_get_data( ooc_cast( list_get_item( iterator ), Foo ) );
		check[ index ] = index;
	}
	
	for( i=0; i<check_size; i++ )
		assertTrue( check[ i ] == i );

	ooc_free( ooc_pass( check ) );
}

static
void
prepend( ListTest self )
{
	static const int list_size = 100;
	int i;
	ListIterator    iterator;
	
	for( i=0; i<list_size; i++ )
		list_prepend( self->foolist, foo_new_with_data( i ) );
		
	for( i=0, iterator = list_first( self->foolist ); i<list_size; i++, iterator = list_next( self->foolist, iterator ) )
		assertTrue( foo_get_data( ooc_cast( list_get_item( iterator ), Foo ) ) == list_size - i - 1 );
}

static
void
paralell_prepend( ListTest self )
{
	static const int check_size = 100;
	ListIterator    iterator;
	int i;
	int * check = ooc_calloc( check_size, sizeof( int ) );
	
	ooc_manage( check, ooc_free );
	
	for( i=0; i<check_size; i++ )
		check[ i ] = 0;

	#ifdef _OPENMP
	#pragma omp parallel for private(i) schedule( guided, 10 )
	#endif
	for( i=0; i<check_size; i++ )
		list_prepend( self->foolist, foo_new_with_data( i ) );

	for( i=0, iterator = list_first( self->foolist ); i<check_size; i++, iterator = list_next( self->foolist, iterator ) )
	{
		int index;
		index = foo_get_data( ooc_cast( list_get_item( iterator ), Foo ) );
		check[ index ] = index;
	}
	
	for( i=0; i<check_size; i++ )
		assertTrue( check[ i ] == i );

	ooc_free( ooc_pass( check ) );
}

static
void
insert_before( ListTest self )
{
	ListIterator iterator;
	
	list_append( self->foolist, foo_new_with_data( 1 ) );
	list_append( self->foolist, foo_new_with_data( 3 ) );
	
	iterator = list_first( self->foolist );
	iterator = list_next( self->foolist, iterator );

	list_insert_before( self->foolist, iterator, foo_new_with_data( 2 ) );

	iterator = list_first( self->foolist );

	list_insert_before( self->foolist, iterator, foo_new_with_data( 0 ) );
	
	check_sequential( self->foolist );
}

static
void
insert_after( ListTest self )
{
	ListIterator iterator;
	
	list_append( self->foolist, foo_new_with_data( 0 ) );
	list_append( self->foolist, foo_new_with_data( 2 ) );
	
	iterator = list_first( self->foolist );

	list_insert_after( self->foolist, iterator, foo_new_with_data( 1 ) );
	
	iterator = list_last( self->foolist );

	list_insert_after( self->foolist, iterator, foo_new_with_data( 3 ) );
	
	check_sequential( self->foolist );
}

static
void
insert_first( ListTest self )
{
	list_insert_before( self->foolist, list_first(self->foolist), foo_new_with_data( 1 ) );
	list_insert_before( self->foolist, list_first(self->foolist), foo_new_with_data( 0 ) );

	check_sequential( self->foolist );
}

static
void
insert_last( ListTest self )
{
	list_insert_after( self->foolist, list_last(self->foolist), foo_new_with_data( 0 ) );
	list_insert_after( self->foolist, list_last(self->foolist), foo_new_with_data( 1 ) );

	check_sequential( self->foolist );
}

static
void
remove_item( ListTest self )
{
	ListIterator 	iterator;
	Foo				foo;
	
	list_append( self->foolist, foo_new_with_data( 0 ) );
	list_append( self->foolist, foo_new_with_data( 1 ) );
	list_append( self->foolist, foo_new_with_data( 1 ) );
	list_append( self->foolist, foo_new_with_data( 2 ) );
	
	iterator = list_first( self->foolist );
	iterator = list_next( self->foolist, iterator );
	iterator = list_next( self->foolist, iterator );
	

	foo = list_remove_item( self->foolist, iterator );
	ooc_delete( (Object) foo );
		
	check_sequential( self->foolist );	
}

static
void
remove_first_item( ListTest self )
{
	Foo				foo;
	
	list_append( self->foolist, foo_new_with_data( 34 ) );
	list_append( self->foolist, foo_new_with_data( 0 ) );
	list_append( self->foolist, foo_new_with_data( 1 ) );
	list_append( self->foolist, foo_new_with_data( 2 ) );
	
	foo = list_remove_first_item( self->foolist );
	ooc_delete( (Object) foo );
		
	check_sequential( self->foolist );	
}

static
void
remove_last_item( ListTest self )
{
	Foo				foo;
	
	list_append( self->foolist, foo_new_with_data( 0 ) );
	list_append( self->foolist, foo_new_with_data( 1 ) );
	list_append( self->foolist, foo_new_with_data( 2 ) );
	list_append( self->foolist, foo_new_with_data( 34 ) );
	
	foo = list_remove_last_item( self->foolist );
	ooc_delete( (Object) foo );
		
	check_sequential( self->foolist );	
}

void
remove_first_item_empty( ListTest self )
{
	assertNull( list_remove_first_item( self->foolist ) );
}

void
remove_last_item_empty( ListTest self )
{
	assertNull( list_remove_last_item( self->foolist ) );
}

static
void
remove_item_single( ListTest self )
{
	Foo foo;
	
	list_append( self->foolist, foo_new_with_data( 0 ) );

	foo = list_remove_item( self->foolist, list_first( self->foolist ) );
	
	assertNull( self->foolist->first );
	assertNull( self->foolist->last );
	
	ooc_delete( (Object) foo );
}

static
void
delete_item( ListTest self )
{
	ListIterator 	iterator;
	
	list_append( self->foolist, foo_new_with_data( 0 ) );
	list_append( self->foolist, foo_new_with_data( 1 ) );
	list_append( self->foolist, foo_new_with_data( 1 ) );
	list_append( self->foolist, foo_new_with_data( 2 ) );
	
	iterator = list_first( self->foolist );
	iterator = list_next( self->foolist, iterator );
	iterator = list_next( self->foolist, iterator );

	list_delete_item( self->foolist, iterator );
		
	check_sequential( self->foolist );	
}

static
void
delete_item_single( ListTest self )
{
	list_append( self->foolist, foo_new_with_data( 0 ) );

	list_delete_item( self->foolist, list_first( self->foolist ) );
	
	assertNull( self->foolist->first );
	assertNull( self->foolist->last );
}

static
void
unmanaged( ListTest self )
{
	Foo foo1, foo2;
	FooNode foonode1, foonode2;
	static char * const string3 = "string 3";
	
	list_append( self->list_um, "string 0" );
	list_append( self->list_um, "string 1" );
	list_append( self->list_um, "string 2" );
	list_append( self->list_um, string3 );
	assertTrue( list_get_item( list_last( self->list_um ) ) == string3 );
	/* destructor must not leak */
	ooc_delete_and_null( (Object*) &self->list_um );
	
	foo1 = foo_new();
	foo2 = foo_new();
	list_append( self->foolist_um, foo1 );
	list_append( self->foolist_um, foo2 );
	assertTrue( list_get_item( list_last( self->foolist_um ) ) == foo2 );
	/* destructor must not leak */
	ooc_delete_and_null( (Object*) &self->foolist_um );
	ooc_delete( (Object) foo1 );
	ooc_delete( (Object) foo2 );	

	foonode1 = foonode_new();
	foonode2 = foonode_new();
	list_append( self->foonodelist_um, foonode1 );
	list_append( self->foonodelist_um, foonode2 );
	assertTrue( list_get_item( list_last( self->foonodelist_um ) ) == foonode2 );
	assertTrue( self->foonodelist_um->last == (void*) foonode2 );
	/* destructor must not leak */
	ooc_delete_and_null( (Object*) &self->foonodelist_um );
	ooc_delete( (Object) foonode1 );
	ooc_delete( (Object) foonode2 );
}

static
void
swap_1( ListTest self )
{
	ListIterator i1, i2, i3, i4;
	
	i1 = list_append( self->foolist, foo_new_with_data( 1 ) );
	i2 = list_append( self->foolist, foo_new_with_data( 0 ) );
	i3 = list_append( self->foolist, foo_new_with_data( 3 ) );
	i4 = list_append( self->foolist, foo_new_with_data( 2 ) );
	
	list_swap( self->foolist, i1, i2 );
	list_swap( self->foolist, i3, i4 );

	check_sequential( self->foolist );
}

static
void
swap_2( ListTest self )
{
	ListIterator i1, i2, i3, i4;
	
	i1 = list_append( self->foolist, foo_new_with_data( 3 ) );
	i2 = list_append( self->foolist, foo_new_with_data( 2 ) );
	i3 = list_append( self->foolist, foo_new_with_data( 1 ) );
	i4 = list_append( self->foolist, foo_new_with_data( 0 ) );
	
	list_swap( self->foolist, i1, i4 );
	list_swap( self->foolist, i2, i3 );

	check_sequential( self->foolist );
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

static char * const foreach_param 		= "foreach parameter";
static int 			foreach_counter;
Class				foreach_expected_item_type;

#define FOREACH_LOOP 60

static
void
item_executor( void * item, void * param )
{
	assertTrue( param == foreach_param );
	assertTrue( ooc_get_type( item ) == foreach_expected_item_type );
	
	if( ooc_isInstanceOf( item, Foo ) ) {
		assertTrue( foo_get_data( ooc_cast( item, Foo ) ) == foreach_counter );
		}
	else if( ooc_isInstanceOf( item, FooNode ) ) {
		assertTrue( foonode_get_data( ooc_cast( item, FooNode ) ) == foreach_counter );
		}	
	else
		failMsg("Unknown item type in list");
		
	foreach_counter++;
}

static
void
foreach( ListTest self )
{
	int i;
	
	/* check for an empty list */
	foreach_expected_item_type = NULL;
	foreach_counter = 0;
	list_foreach( self->foolist, item_executor, foreach_param );
	assertTrue( foreach_counter == 0 );
	list_foreach( self->foonodelist, item_executor, foreach_param );
	assertTrue( foreach_counter == 0 );

	/* check with an ordinary data type */
	for( i=0; i<FOREACH_LOOP; i++ )
		list_append( self->foolist, foo_new_with_data( i ) );
		
	foreach_expected_item_type = &FooClass;
	foreach_counter = 0;
	list_foreach( self->foolist, item_executor, foreach_param );
	assertTrue( foreach_counter == FOREACH_LOOP );
	
	/* check with a ListNode data type */
	for( i=0; i<FOREACH_LOOP; i++ )
		list_append( self->foonodelist, foonode_new_with_data( i ) );
		
	foreach_expected_item_type = &FooNodeClass;
	foreach_counter = 0;
	list_foreach( self->foonodelist, item_executor, foreach_param );
	assertTrue( foreach_counter == FOREACH_LOOP );
}

static int expected_value;

static
int
item_checker( void * item, void * param )
{
	int current_value = -1;
	
	assertTrue( param == foreach_param );
	assertTrue( ooc_get_type( item ) == foreach_expected_item_type );
	
	if( ooc_isInstanceOf( item, Foo ) )
		current_value = foo_get_data( ooc_cast( item, Foo ) );
	else if( ooc_isInstanceOf( item, FooNode ) )
		current_value = foonode_get_data( ooc_cast( item, FooNode ) );
	else
		failMsg("Unknown item type in list");
		
	assertTrue( current_value == foreach_counter );
	
	foreach_counter++;
	
	return current_value == expected_value;
}

static
int
foo_comparator( void * item, void * param )
{
	assertTrue( param == foreach_param );
	assertTrue( ooc_get_type( item ) == &FooClass );
	
	return foo_get_data( ooc_cast( item, Foo ) ) == expected_value;
}

static
void
foreach_until_true( ListTest self )
{
	int i;
	ListIterator found;
	
	/* check for an empty list */
	foreach_expected_item_type = NULL;
	foreach_counter = 0;
	expected_value = FOREACH_LOOP/3;
	found = list_foreach_until_true( self->foolist, list_first( self->foolist ), item_checker, foreach_param );
	assertTrue( foreach_counter == 0 );
	assertNull( found );
	found = list_foreach_until_true( self->foonodelist, list_first( self->foolist ), item_checker, foreach_param );
	assertTrue( foreach_counter == 0 );
	assertNull( found );

	/* check with an ordinary data type */
	for( i=0; i<FOREACH_LOOP; i++ )
		list_append( self->foolist, foo_new_with_data( i ) );
		
	foreach_expected_item_type = &FooClass;
	foreach_counter = 0;
	expected_value = FOREACH_LOOP/3;
	found = list_foreach_until_true( self->foolist, list_first( self->foolist ), item_checker, foreach_param );
	assertTrue( foreach_counter == FOREACH_LOOP/3 + 1 );
	assertTrue( foo_get_data( list_get_item( found ) ) == FOREACH_LOOP/3 );

	expected_value = FOREACH_LOOP+1;
	foreach_counter = 0;
	found = list_foreach_until_true( self->foolist, list_first( self->foolist ), item_checker, foreach_param );
	assertTrue( foreach_counter == FOREACH_LOOP );
	assertNull( found );
	
	expected_value = FOREACH_LOOP/3;
	foreach_counter = FOREACH_LOOP-1;
	found = list_foreach_until_true( self->foolist, list_last( self->foolist ), item_checker, foreach_param );
	assertTrue( foreach_counter == FOREACH_LOOP );
	assertNull( found );
	
	/* check with a ListNode data type */
	for( i=0; i<FOREACH_LOOP; i++ )
		list_append( self->foonodelist, foonode_new_with_data( i ) );
		
	foreach_expected_item_type = &FooNodeClass;
	foreach_counter = 0;
	expected_value = FOREACH_LOOP/4;
	found = list_foreach_until_true( self->foonodelist, list_first( self->foonodelist ), item_checker, foreach_param );
	assertTrue( foreach_counter == FOREACH_LOOP/4 + 1 );
	assertTrue( foonode_get_data( list_get_item( found ) ) == FOREACH_LOOP/4 );

	expected_value = FOREACH_LOOP+1;
	foreach_counter = 0;
	found = list_foreach_until_true( self->foonodelist, list_first( self->foonodelist ), item_checker, foreach_param );
	assertTrue( foreach_counter == FOREACH_LOOP );
	assertNull( found );

	expected_value = FOREACH_LOOP/4;
	foreach_counter = FOREACH_LOOP-1;
	found = list_foreach_until_true( self->foonodelist, list_last( self->foonodelist ), item_checker, foreach_param );
	assertTrue( foreach_counter == FOREACH_LOOP );
	assertNull( found );
}

static
void
foreach_delete_if( ListTest self )
{
	int i, k;
	
	expected_value = FOREACH_LOOP+1;
	
	for( i=0; i<FOREACH_LOOP; i++ )
	{
if( i == 85 )
	k= i;
		list_append( self->foolist, foo_new_with_data( i ) );
		if( i % 17 != 0 )
			list_append( self->foolist, foo_new_with_data( expected_value ) );
	}
	
	foreach_expected_item_type = &FooClass;
	list_foreach_delete_if( self->foolist, foo_comparator, foreach_param );
	check_sequential( self->foolist );
}

static
void
find_item( ListTest self )
{
	int i, to_be_found, found_counter;
	ListIterator found;
	
	expected_value = FOREACH_LOOP+1;
	foreach_expected_item_type = &FooClass;
	
	for( i=0, to_be_found= 0; i<FOREACH_LOOP; i++ )
	{
		list_append( self->foolist, foo_new_with_data( i ) );
		if( i % 17 != 0 ) {
			list_append( self->foolist, foo_new_with_data( expected_value ) );
			to_be_found++;
		}
	}
	
	found_counter = 0;
	found = NULL;
	do {
		found = list_find_item( self->foolist, found, foo_comparator, foreach_param );
		if( found )
		{
			found_counter++;
			assertTrue( foo_get_data( ooc_cast( list_get_item( found ), Foo ) ) == expected_value );
			found = list_next( self->foolist, found );
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
	foreach_expected_item_type = &FooClass;
	
	for( i=0, to_be_found= 0; i<FOREACH_LOOP; i++ )
	{
		list_append( self->foolist, foo_new_with_data( i ) );
		if( i % 17 != 0 ) {
			list_append( self->foolist, foo_new_with_data( expected_value ) );
			to_be_found++;
		}
	}
	
	found_counter = 0;
	found = NULL;
	do {
		found = list_find_item_reverse( self->foolist, found, foo_comparator, foreach_param );
		if( found )
		{
			found_counter++;
			assertTrue( foo_get_data( ooc_cast( list_get_item( found ), Foo ) ) == expected_value );
			found = list_previous( self->foolist, found );
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

static
void
duplicate_untyped( ListTest self )
{
	List volatile duplicate = NULL;

	try {
		duplicate = (List) ooc_duplicate( (Object) self->list );
		fail();
		}
	catch_any {
		assertTrue( exception_get_error_code( exception ) == err_can_not_be_duplicated );
		assertNull( duplicate );
		}
	end_try;
}

static
void
duplicate_none_nodes( ListTest self )
{
	List volatile duplicate = NULL;

	list_append( self->foolist, foo_new_with_data( 0 ) );
	list_append( self->foolist, foo_new_with_data( 1 ) );
	list_append( self->foolist, foo_new_with_data( 2 ) );
	list_append( self->foolist, foo_new_with_data( 3 ) );

	duplicate = (List) ooc_duplicate( (Object) self->foolist );

	ooc_delete_and_null( (Object *) & self->foolist );

	assertTrue( duplicate->destroy == (list_item_destroyer) ooc_delete );
	assertTrue( duplicate->type == &FooClass );
	assertFalse( duplicate->list_of_nodes );
	assertNotNull( duplicate->first );
	assertNotNull( duplicate->last );

	check_sequential( duplicate );

	ooc_delete( (Object) duplicate );
}

static
void
duplicate_nodes( ListTest self )
{
	List volatile duplicate = NULL;

	list_append( self->foonodelist, foonode_new_with_data( 0 ) );
	list_append( self->foonodelist, foonode_new_with_data( 1 ) );
	list_append( self->foonodelist, foonode_new_with_data( 2 ) );
	list_append( self->foonodelist, foonode_new_with_data( 3 ) );

	duplicate = (List) ooc_duplicate( (Object) self->foonodelist );

	ooc_delete_and_null( (Object *) & self->foonodelist );

	assertTrue( duplicate->destroy == (list_item_destroyer) ooc_delete );
	assertTrue( duplicate->type == &FooNodeClass );
	assertTrue( duplicate->list_of_nodes );
	assertNotNull( duplicate->first );
	assertNotNull( duplicate->last );

	check_sequential( duplicate );

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
	TEST(swap_1),
	TEST(swap_2),
	TEST(swap_3),
	TEST(swap_4),
	TEST(swap_5),
	TEST(foreach),
	TEST(foreach_until_true),
	TEST(foreach_delete_if),
	TEST(find_item),
	TEST(find_item_reverse),
	TEST(bad_reuse),
	TEST(duplicate_untyped),
	TEST(duplicate_none_nodes),
	TEST(duplicate_nodes),
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
TESTCASE_MAIN
{
	ListTest listtest;
	int result;
	
	ooc_init_class( ListTest );
	listtest = ooc_new( ListTest, &methods );
	result = testcase_run((TestCase)listtest);
	ooc_delete( (Object) listtest );
	ooc_finalize_all();
	return result;
}

