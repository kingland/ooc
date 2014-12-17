
/* This is a OocTest class implementation file
 */

#include <string.h>

#include "../libs/testcase.h"

#include "implement/foo.h"
#include "implement/bar.h"
#include "implement/barson.h"
#include "implement/bardaughter.h"

#ifdef OOC_NO_FINALIZE
#define ooc_finalize_class( x )
#define ooc_finalize_all( )
#endif


/** @class OocTest
 *  @brief OocTest class - brief description.
 * 	@see ooctest.h
 */

/** @file ooctest.h 
 *  @brief OocTest class - brief description.
 * OocTest is a .....
 * @note	This class is a final class, can not be inherited.
 * @note	Run as: valgrind --leak-check=yes --quiet ./ooctest
 */ 

DeclareClass( OocTest, TestCase );

Virtuals( OocTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( OocTest, TestCase )

	int		data;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( OocTest, TestCase );


/* Class initializing
 */

static void ooctest_before_class( OocTest self );
static void ooctest_before( OocTest self );
static void ooctest_after( OocTest self );
static void ooctest_after_class( OocTest self );

static
void
OocTest_initialize( Class this )
{
	OocTestVtable vtab = & OocTestVtableInstance;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) ooctest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) ooctest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) ooctest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) ooctest_after_class;

	ooc_init_class( Exception );
	
	/* Allocate global resources here */
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE

static
void
OocTest_finalize( Class this )
{
	/* Release global resources! */
}

#endif

/* Constructor
 */

static
void
OocTest_constructor( OocTest self, const void * params )
{
	assert( ooc_isInitialized( OocTest ) );
	
	chain_constructor( OocTest, self, params );
}

/* Destructor
 */

static
void
OocTest_destructor( OocTest self, OocTestVtable vtab )
{
}

/* Copy constuctor
 */

static
int
OocTest_copy( OocTest self, const OocTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
ooctest_before_class( OocTest self )
{
	return;
}

static
void
ooctest_before( OocTest self )
{
	return;
}

static
void
ooctest_after( OocTest self )
{
	return;
}

static
void
ooctest_after_class( OocTest self )
{
	return;
}


/*	=====================================================
	Test methods
 */

/* This is a BarBadGrandSon class implementation
 */

DeclareClass( BarBadGrandSon, BarSon );

ClassMembers( BarBadGrandSon, BarSon )
EndOfClassMembers;

Virtuals( BarBadGrandSon, BarSon )
	void (* uninitialized_virtual)( void );
EndOfVirtuals;

AllocateClass( BarBadGrandSon, BarSon );

static	void	BarBadGrandSon_initialize( Class this ) {}
#ifndef OOC_NO_FINALIZE
static	void	BarBadGrandSon_finalize( Class this ) {}
#endif
static	void	BarBadGrandSon_constructor( BarBadGrandSon self, const void * params ) {}
static	void	BarBadGrandSon_destructor( BarBadGrandSon self, BarBadGrandSonVtable vtab ) {}
static	int		BarBadGrandSon_copy( BarBadGrandSon self, const BarBadGrandSon from ) { return OOC_COPY_DEFAULT; }


static
void
test_class_table( void )
{
	assertTrue( FooClass.size == sizeof(struct FooObject)/sizeof(char) );
	assertZero( strcmp( FooClass.c.type.name, "Foo" ) );
	assertTrue( FooClass.parent == & BaseClass );
	assertNotNull( FooClass.c.vtable );
	assertTrue( FooClass.vtab_size == sizeof(struct FooVtable_stru)/sizeof(char) );

	assertTrue( BarClass.size == sizeof(struct BarObject)/sizeof(char) );
	assertZero( strcmp( BarClass.c.type.name, "Bar" ) );
	assertTrue( BarClass.parent == & BaseClass );
	assertNotNull( BarClass.c.vtable );
	assertTrue( BarClass.vtab_size == sizeof(struct BarVtable_stru)/sizeof(char) );

	assertTrue( BarSonClass.size == sizeof(struct BarSonObject)/sizeof(char) );
	assertZero( strcmp( BarSonClass.c.type.name, "BarSon" ) );
	assertTrue( BarSonClass.parent == & BarClass );
	assertNotNull( BarSonClass.c.vtable );
	
	assertTrue( BarBadGrandSonClass.vtab_size == sizeof(struct BarBadGrandSonVtable_stru)/sizeof(char) );
	assertTrue( BarBadGrandSonClass.size == sizeof(struct BarBadGrandSonObject)/sizeof(char) );
	assertZero( strcmp( BarBadGrandSonClass.c.type.name, "BarBadGrandSon" ) );
	assertTrue( BarBadGrandSonClass.parent == & BarSonClass );
	assertTrue( BarBadGrandSonClass.c.vtable == (Vtable) & BarBadGrandSonVtableInstance);
	assertTrue( BarBadGrandSonClass.vtab_size == sizeof(struct BarBadGrandSonVtable_stru)/sizeof(char) );
	assertTrue( BarBadGrandSonClass.init == BarBadGrandSon_initialize );
	#ifndef OOC_NO_FINALIZE
	assertTrue( BarBadGrandSonClass.c.finz == (void (*)(ClassCommons)) BarBadGrandSon_finalize );
	#endif
	assertTrue( BarBadGrandSonClass.ctor == (void (*)( Object, const void *))	BarBadGrandSon_constructor );
	assertTrue( BarBadGrandSonClass.dtor == (void (*)( Object, Vtable))			BarBadGrandSon_destructor );
	assertTrue( BarBadGrandSonClass.copy == (int  (*)( Object, const Object))	BarBadGrandSon_copy );
}

static
void
test_init( void )
{
	assertTrue( ooc_isInitialized( OocTest ) );
	
	ooc_init_class( Foo );
	ooc_init_class( BarSon );

	assertTrue( ooc_isInitialized( Foo ) );
	assertTrue( ooc_isInitialized( Bar ) ); /* Parent must be initialized automatically */
	assertTrue( ooc_isInitialized( BarSon ) );
	
	/* Check if vtable has valid Class pointer */
	assertTrue( ((BaseVtable)( FooClass.c.vtable ))->_class == & FooClass );
	assertTrue( ((BaseVtable)( BarClass.c.vtable ))->_class == & BarClass );
	assertTrue( ((BaseVtable)( BarSonClass.c.vtable ))->_class == & BarSonClass );

	/* Check if vtable has no destroy_check operator */
	assertNull( ((BaseVtable)( FooClass.c.vtable ))->_destroy_check );
	assertNull( ((BaseVtable)( BarClass.c.vtable ))->_destroy_check );
	assertNull( ((BaseVtable)( BarSonClass.c.vtable ))->_destroy_check );
	
	/* Ckeck if the vtable is inherited */
	assertTrue( ((BarVtable)( BarClass.c.vtable ))->bar_virtual ==
				((BarSonVtable)( BarSonClass.c.vtable ))->Bar.bar_virtual	);
				
	#ifndef OOC_NO_FINALIZE
	/* Ckeck if class initialization chain is correct */
	assertNotNull( ((BaseVtable)( FooClass.c.vtable ))->_class_register_prev );
	assertTrue( ((BaseVtable)( FooClass.c.vtable ))->_class_register_next == (ClassCommons) & BarClass );
	assertTrue( ((BaseVtable)( BarClass.c.vtable ))->_class_register_prev == (ClassCommons) & FooClass );
	assertTrue( ((BaseVtable)( BarClass.c.vtable ))->_class_register_next == (ClassCommons) & BarSonClass );
	assertTrue( ((BaseVtable)( BarSonClass.c.vtable ))->_class_register_prev == (ClassCommons) & BarClass );
	assertNull( ((BaseVtable)( BarSonClass.c.vtable ))->_class_register_next );
				
	ooc_finalize_class( BarSon );
	ooc_finalize_class( Bar );
	ooc_finalize_class( Foo );
	
	assertFalse( ooc_isInitialized( Foo ) );
	assertFalse( ooc_isInitialized( Bar ) );
	assertFalse( ooc_isInitialized( BarSon ) );
	#endif
}

void
static
test_finalize( void )
{
#ifndef OOC_NO_FINALIZE
	assertTrue( ooc_isInitialized( OocTest ) );
	
	ooc_init_class( Foo );
	ooc_init_class( BarSon );

	assertTrue( ooc_isInitialized( Foo ) );
	assertTrue( ooc_isInitialized( Bar ) ); /* Parent must be initialized automatically */
	assertTrue( ooc_isInitialized( BarSon ) );
	
	/* Ckeck if class initialization chain is correct */
	assertNotNull( ((BaseVtable)( FooClass.c.vtable ))->_class_register_prev );
	assertTrue( ((BaseVtable)( FooClass.c.vtable ))->_class_register_next == (ClassCommons) & BarClass );
	assertTrue( ((BaseVtable)( BarClass.c.vtable ))->_class_register_prev == (ClassCommons) & FooClass );
	assertTrue( ((BaseVtable)( BarClass.c.vtable ))->_class_register_next == (ClassCommons) & BarSonClass );
	assertTrue( ((BaseVtable)( BarSonClass.c.vtable ))->_class_register_prev == (ClassCommons) & BarClass );
	assertNull( ((BaseVtable)( BarSonClass.c.vtable ))->_class_register_next );
				
	ooc_finalize_class( Bar );
	
	/* Ckeck if class initialization chain is correct */
	assertNotNull( ((BaseVtable)( FooClass.c.vtable ))->_class_register_prev );
	assertTrue( ((BaseVtable)( FooClass.c.vtable ))->_class_register_next == (ClassCommons) & BarSonClass );
	assertNull( ((BaseVtable)( BarClass.c.vtable ))->_class_register_prev );
	assertNull( ((BaseVtable)( BarClass.c.vtable ))->_class_register_next );
	assertTrue( ((BaseVtable)( BarSonClass.c.vtable ))->_class_register_prev == (ClassCommons) & FooClass );
	assertNull( ((BaseVtable)( BarSonClass.c.vtable ))->_class_register_next );
	
	ooc_finalize_class( BarSon );
	ooc_finalize_class( Foo );
	
	/* Ckeck if class initialization chain is correct */
	assertNull( ((BaseVtable)( FooClass.c.vtable ))->_class_register_prev );
	assertNull( ((BaseVtable)( FooClass.c.vtable ))->_class_register_next );
	assertNull( ((BaseVtable)( BarClass.c.vtable ))->_class_register_prev );
	assertNull( ((BaseVtable)( BarClass.c.vtable ))->_class_register_next );
	assertNull( ((BaseVtable)( BarSonClass.c.vtable ))->_class_register_prev );
	assertNull( ((BaseVtable)( BarSonClass.c.vtable ))->_class_register_next );

	assertFalse( ooc_isInitialized( Foo ) );
	assertFalse( ooc_isInitialized( Bar ) );
	assertFalse( ooc_isInitialized( BarSon ) );
#endif
}

static
void
test_virtual_initialization( void )
{
	BarBadGrandSon bbgs;
	
	ooc_init_class( BarBadGrandSon );
	
	bbgs = (BarBadGrandSon) ooc_new( BarBadGrandSon, NULL );
	{
		ooc_manage_object( bbgs );
	
		BarBadGrandSonVirtual( bbgs )->BarSon.barson_virtual( (BarSon) bbgs );
		BarBadGrandSonVirtual( bbgs )->BarSon.Bar.bar_virtual( (Bar) bbgs );
		
		try {
			BarBadGrandSonVirtual( bbgs )->uninitialized_virtual();
			fail();
		}
		catch_any
			assertTrue( exception_get_error_code( exception ) == err_undefined_virtual );
		end_try;
		
		ooc_delete( ooc_pass( (Object) bbgs ) );
	}
	
	ooc_finalize_class( BarBadGrandSon );
}

/* Testing class lifecycle
 */

DeclareClass( FooLife, Base );

ClassMembers( FooLife, Base )
EndOfClassMembers;

Virtuals( FooLife, Base )
EndOfVirtuals;

AllocateClass( FooLife, Base );

enum
foolife_status
{
	foolife_nothing,
	foolife_init,
	foolife_ctor,
	foolife_copy,
	foolife_dtor,
	foolife_finz
};

static enum foolife_status foolife_last_called;

static	void	FooLife_initialize( Class this ) 	{ foolife_last_called++; }
#ifndef OOC_NO_FINALIZE
static	void	FooLife_finalize( Class this ) 		{ foolife_last_called++; }
#endif

static	void	FooLife_constructor( FooLife self, const void * params )
													{  foolife_last_called++; }

static	void	FooLife_destructor( FooLife self, FooLifeVtable vtab )
													{ foolife_last_called++; }
static	int	FooLife_copy( FooLife self, const FooLife from )
													{ foolife_last_called++;
													  return OOC_COPY_DEFAULT; }

void
test_lifecycle( void )
{
	FooLife foolife1, foolife2;
	
		foolife_last_called = foolife_nothing;
	ooc_init_class( FooLife );
		assertTrue( foolife_last_called == foolife_init );
	foolife1 = (FooLife) ooc_new( FooLife, NULL );
		assertTrue( foolife_last_called == foolife_ctor );
	foolife2 = (FooLife) ooc_duplicate( (Object) foolife1 );
		assertTrue( foolife_last_called == foolife_copy );
	ooc_delete( (Object) foolife1 );
		assertTrue( foolife_last_called == foolife_dtor );
	#ifndef OOC_NO_FINALIZE
	ooc_finalize_class( FooLife );
		assertTrue( foolife_last_called == foolife_finz );
	#endif

	/* prevent leaking foolife2 */
	ooc_init_class( FooLife );
	ooc_delete( (Object) foolife2 );
	ooc_finalize_class( FooLife );
}

static
void
test_new( void )
{
	Foo foo;
	
	foo = foo_new();
	assertNull( foo->text );
	assertZero( foo->data );
	ooc_delete( (Object) foo );
	
	foo = foo_new_with_data( 100 );
	assertNull( foo->text );
	assertTrue( foo->data == 100 );
	ooc_delete( (Object) foo );

	foo = foo_new_with_const_text( "copyable test text" );
	assertZero( strcmp( foo->text, "copyable test text" ) );
	assertZero( foo->data );
	ooc_delete( (Object) foo );
}

/* Test constructors and inheritance
 */
 
static const char * ctorcheck_params = "CtorCheck parameters";

DeclareClass( CtorCheck, Base );

ClassMembers( CtorCheck, Base )
	int ctor_called;
	int	dtor_called;
	int copy_called;
	int child_ctor_called;
	int	child_dtor_called;
	int child_copy_called;
	int copy_operation;
	int	data;
EndOfClassMembers;

Virtuals( CtorCheck, Base )
EndOfVirtuals;

AllocateClass( CtorCheck, Base );

DeclareClass( CtorCheckSon, CtorCheck );

#define CCS_ARRAY_LEN 100

ClassMembers( CtorCheckSon, CtorCheck )
	int		array[ CCS_ARRAY_LEN ];
EndOfClassMembers;

Virtuals( CtorCheckSon, CtorCheck )
EndOfVirtuals;

AllocateClass( CtorCheckSon, CtorCheck );

static	void	CtorCheck_initialize( Class this ) {}
#ifndef OOC_NO_FINALIZE
static	void	CtorCheck_finalize( Class this ) {}
#endif

static	void	CtorCheck_constructor( CtorCheck self, const void * params )
{
	assertTrue( ooc_isInitialized( CtorCheck ) );
	assertTrue( params == ctorcheck_params );
	assertFalse( self->ctor_called );
	self->ctor_called = TRUE;
}
static	void	CtorCheck_destructor( CtorCheck self, CtorCheckVtable vtab )
{
	assertNull( self->Base._vtab );
	assertTrue( vtab == (CtorCheckVtable) & CtorCheckSonVtableInstance );
	assertTrue( self->child_dtor_called );
	assertFalse( self->dtor_called );
	self->dtor_called = TRUE;
}
static	int		CtorCheck_copy( CtorCheck self, const CtorCheck from )
{
	assertFalse( self->copy_called );
	self->copy_called = TRUE;
	return OOC_COPY_DONE;
}

static	void	CtorCheckSon_initialize( Class this ) {}
#ifndef OOC_NO_FINALIZE
static	void	CtorCheckSon_finalize( Class this ) {}
#endif

static	void	CtorCheckSon_constructor( CtorCheckSon self, const void * params )
{
	assertTrue( ooc_isInitialized( CtorCheckSon ) );
	assertTrue( params == ctorcheck_params );
	assertFalse( self->CtorCheck.child_ctor_called );	
	assertFalse( self->CtorCheck.ctor_called );
	chain_constructor( CtorCheckSon, self, params );
	assertTrue( self->CtorCheck.ctor_called );
	self->CtorCheck.child_ctor_called = TRUE;
}

static	void	CtorCheckSon_destructor( CtorCheckSon self, CtorCheckSonVtable vtab )
{
	assertNull( self->CtorCheck.Base._vtab );
	assertTrue( vtab == & CtorCheckSonVtableInstance );
	assertFalse( self->CtorCheck.child_dtor_called );
	assertFalse( self->CtorCheck.dtor_called );
	self->CtorCheck.child_dtor_called = TRUE;
}

static	int		CtorCheckSon_copy( CtorCheckSon self, const CtorCheckSon from )
{
	assertTrue( self->CtorCheck.copy_called );
	self->CtorCheck.child_copy_called = TRUE;
	return from->CtorCheck.copy_operation;
}

static
void
test_ctor_new( void )
{
	CtorCheck ccs;
	
	ooc_init_class(	CtorCheckSon );
	
	ccs = (CtorCheck) ooc_new( CtorCheckSon, ctorcheck_params );
	assertTrue( ooc_isInstanceOf( ccs, CtorCheckSon ) );
	assertTrue( ccs->ctor_called );
	assertTrue( ccs->child_ctor_called );
	assertFalse( ccs->dtor_called );
	assertFalse( ccs->child_dtor_called );
	assertZero( ccs->data );
	
	ooc_release( (Object) ccs );
	assertTrue( ccs->dtor_called );
	assertTrue( ccs->child_dtor_called );

	ooc_free( ccs );
	ooc_finalize_class( CtorCheckSon );
}

static
void
test_ctor_use( void )
{
	CtorCheck ccs;
	
	ooc_init_class(	CtorCheckSon );
	
	ccs = (CtorCheck) ooc_malloc( sizeof(struct CtorCheckSonObject)/sizeof(char) );
	memset( ccs, 0xAA, sizeof(struct CtorCheckSonObject)/sizeof(char) );
	
	ooc_use( ccs, CtorCheckSon, ctorcheck_params );
	
	assertTrue( ooc_isInstanceOf( ccs, CtorCheckSon ) );
	assertTrue( ccs->ctor_called );
	assertTrue( ccs->child_ctor_called );
	assertFalse( ccs->dtor_called );
	assertFalse( ccs->child_dtor_called );
	assertZero( ccs->data );
	
	ooc_release( (Object) ccs );
	assertTrue( ccs->dtor_called );
	assertTrue( ccs->child_dtor_called );

	ooc_free( ccs );
	ooc_finalize_class( CtorCheckSon );
}

static
void
test_copy_null( void )
{
	assertNull( ooc_duplicate( NULL ) );
}

static
void
test_copy( void )
{
	CtorCheckSon ccs, duplicate;
	
	ooc_init_class(	CtorCheckSon );
	
	ccs = (CtorCheckSon) ooc_new( CtorCheckSon, ctorcheck_params );
	memset( & ccs->array, 0xAA, CCS_ARRAY_LEN );

	ccs->CtorCheck.copy_operation = OOC_COPY_DEFAULT;
	duplicate = (CtorCheckSon) ooc_duplicate( (Object) ccs );
	assertNotNull( duplicate );
	assertTrue( ccs != duplicate );
	assertTrue( ooc_isInstanceOf( duplicate, CtorCheckSon ) );
	assertZero( memcmp( &ccs->array, &duplicate->array, CCS_ARRAY_LEN ) );
	ooc_delete_and_null( (Object*) &duplicate );

	ccs->CtorCheck.copy_operation = OOC_COPY_DONE;
	duplicate = (CtorCheckSon) ooc_duplicate( (Object) ccs );
	assertTrue( duplicate->CtorCheck.copy_called );
	assertTrue( duplicate->CtorCheck.child_copy_called );
	ooc_delete_and_null( (Object*) &duplicate );

	ccs->CtorCheck.copy_operation = OOC_NO_COPY;
	try {
		duplicate = (CtorCheckSon) ooc_duplicate( (Object) ccs );
		fail();
	}
	catch_any {
		assertNull( duplicate );
		assertTrue( exception_get_error_code( exception ) == err_can_not_be_duplicated );
	}
	end_try;
	
	ooc_delete( (Object) ccs );
	ooc_finalize_class( CtorCheckSon );
}

static
void
test_delete_and_null( void )
{
	void	*mem, *old, *tmp;
	Foo 	foo;

	mem = ooc_malloc( 100 );
	assertNotNull( mem );
	old = mem;
	tmp = ooc_ptr_read_and_null( & mem );
	assertTrue( old == tmp );
	assertNull( mem );
	ooc_free( tmp );
			
	mem = ooc_malloc( 100 );
	assertNotNull( mem );
	ooc_free_and_null( & mem );
	assertNull( mem );
			
	foo = foo_new();
	assertTrue( ooc_isInstanceOf( foo, Foo ) );
	ooc_delete_and_null( (Object*) &foo );
	assertNull( foo );
}

/* Testing the demolition of circularly referenced objects.
 */

DeclareClass( CircularCheck, Base );

ClassMembers( CircularCheck, Base )
	CircularCheck	reference;
EndOfClassMembers;

Virtuals( CircularCheck, Base )
EndOfVirtuals;

AllocateClass( CircularCheck, Base );

static	void	CircularCheck_initialize( Class this ) {}
#ifndef OOC_NO_FINALIZE
static	void	CircularCheck_finalize( Class this ) {}
#endif
static	void	CircularCheck_constructor( CircularCheck self, const void * params ) {}
static	void	CircularCheck_destructor( CircularCheck self, CircularCheckVtable vtab )
{
	assertNull( self->Base._vtab );
	assertTrue( vtab == & CircularCheckVtableInstance );
	ooc_delete_and_null( (Object *) &self->reference );
}
static	int		CircularCheck_copy( CircularCheck self, const CircularCheck from ) { return OOC_NO_COPY; }
static	void	circularcheck_set_reference( CircularCheck self, CircularCheck reference ) { self->reference = reference; }

static
void
test_delete_circular( void )
{
	CircularCheck first, previous;
	int i;
	
	ooc_init_class( CircularCheck );
	
	first = previous = (CircularCheck) ooc_new( CircularCheck, NULL );
	
	for( i = 0; i < 10; i++ ) 
	{
		CircularCheck cc = (CircularCheck) ooc_new( CircularCheck, NULL );
		circularcheck_set_reference( cc, previous );
		previous = cc;
	}
	circularcheck_set_reference( first, previous );
	
	ooc_delete( (Object) first );
	
	ooc_finalize_class( CircularCheck );
}

static
void
test_cast( void )
{
	Foo 		foo;
	Bar 		bar;
	BarSon 		barson;
	BarDaughter	bardaughter;
	
	ooc_init_class( Foo );
	ooc_init_class( BarSon );
	ooc_init_class( BarDaughter );
	
	assertTrue( ooc_isClassOf( BarSon, Bar ) );
	assertTrue( ooc_isClassOf( BarDaughter, Bar ) );
	assertFalse( ooc_isClassOf( Bar, BarDaughter ) );
	assertFalse( ooc_isClassOf( Foo, Bar ) );

	foo = foo_new();
	bar = bar_new();
	barson = barson_new();
	bardaughter = bardaughter_new();
	
	assertTrue( ooc_isInstanceOf( bar, Bar ) );
	assertTrue( ooc_isInstanceOf( bardaughter, BarDaughter ) );
	assertTrue( ooc_isInstanceOf( barson, BarSon ) );
	assertTrue( ooc_isInstanceOf( bardaughter, Bar ) );
	assertTrue( ooc_isInstanceOf( barson, Bar ) );
	assertFalse( ooc_isInstanceOf( bar, BarSon ) );
	assertFalse( ooc_isInstanceOf( bar, BarDaughter ) );
	assertFalse( ooc_isInstanceOf( bar, Foo ) );
	
	assertTrue( ooc_get_type( (Object) bar ) == &BarClass );
	assertTrue( ooc_get_type( (Object) barson ) == &BarSonClass );
	assertTrue( ooc_get_type( (Object) bardaughter ) == &BarDaughterClass );
	
	assertTrue( ooc_cast( barson, Bar ) == (Bar) barson );
	assertTrue( ooc_cast( bardaughter, Bar ) == (Bar) bardaughter );
	assertTrue( ooc_cast( bar, Bar ) == (Bar) bar );
	
	try {
		ooc_cast( bar, BarSon );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;
	
	try {
		ooc_cast( bardaughter, BarSon );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;
	
	try {
		ooc_cast( bar, Foo );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;
	
	ooc_check_cast( barson, &BarClass );
	ooc_check_cast( bardaughter, &BarClass );
	ooc_check_cast( bar, &BarClass );
	
	try {
		ooc_check_cast( bar, &BarSonClass );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;
	
	try {
		ooc_check_cast( bardaughter, &BarSonClass );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;
	
	try {
		ooc_check_cast( bar, &FooClass );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;
	
	ooc_delete( (Object) foo );
	ooc_delete( (Object) bar );
	ooc_delete( (Object) barson );
	ooc_delete( (Object) bardaughter );
	
	ooc_finalize_class( BarDaughter );
	ooc_finalize_class( BarSon );
	ooc_finalize_class( Bar );
	ooc_finalize_class( Foo );
}

/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
ROM_ALLOC
struct TestCaseMethod methods[] =
{
	
	TEST(test_class_table),
	TEST(test_init),
	TEST(test_finalize),
	TEST(test_lifecycle),
	TEST(test_virtual_initialization),
	TEST(test_new),
	TEST(test_ctor_new),
	TEST(test_ctor_use),
	TEST(test_copy_null),
	TEST(test_copy),
	TEST(test_delete_and_null),
	TEST(test_delete_circular),
	TEST(test_cast),
	
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */

TESTCASE_MAIN
{
	OocTest ooctest;
	int result;

	ooc_init_class( OocTest );
	ooctest = (OocTest) ooc_new( OocTest, &methods );
	result = testcase_run((TestCase)ooctest);
	ooc_delete( (Object) ooctest );
	ooc_finalize_all();
	return result;
}
