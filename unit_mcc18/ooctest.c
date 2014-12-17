
/* This is a OocTest class implementation file
 */

#include <string.h>

#include "../libs/testcase.h"

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

#ifndef __18CXX
#define strcmppgm(a, b) strcmp(a, b)
#define strcmppgm2ram(a, b) strcmp(a, b)
#endif

DeclareClass( OocTest, TestCase );

Virtuals( OocTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( OocTest, TestCase )

	int		data;

EndOfClassMembers;

typedef struct TestCaseObject * TestCase; /* This is not necessary here! MPLAB C18 bug requires this. */


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
	OocTestVtable vtab = (OocTestVtable) this->c.vtable;
	
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
	assertZero( strcmppgm( FooClass.c.type.name, "Foo" ) );
	assertTrue( FooClass.parent == & BaseClass );
	assertNotNull( FooClass.c.vtable );
	assertTrue( FooClass.vtab_size == sizeof(struct FooVtable_stru)/sizeof(char) );

	assertTrue( BarClass.size == sizeof(struct BarObject)/sizeof(char) );
	assertZero( strcmppgm( BarClass.c.type.name, "Bar" ) );
	assertTrue( BarClass.parent == & BaseClass );
	assertNotNull( BarClass.c.vtable );
	assertTrue( BarClass.vtab_size == sizeof(struct BarVtable_stru)/sizeof(char) );

	assertTrue( BarSonClass.size == sizeof(struct BarSonObject)/sizeof(char) );
	assertZero( strcmppgm( BarSonClass.c.type.name, "BarSon" ) );
	assertTrue( BarSonClass.parent == & BarClass );
	assertNotNull( BarSonClass.c.vtable );
	
	assertTrue( BarBadGrandSonClass.vtab_size == sizeof(struct BarBadGrandSonVtable_stru)/sizeof(char) );
	assertTrue( BarBadGrandSonClass.size == sizeof(struct BarBadGrandSonObject)/sizeof(char) );
	assertZero( strcmppgm( BarBadGrandSonClass.c.type.name, "BarBadGrandSon" ) );
	assertTrue( BarBadGrandSonClass.parent == & BarSonClass );
	assertTrue( BarBadGrandSonClass.c.vtable == (Vtable) & BarBadGrandSonVtableInstance);
	assertTrue( BarBadGrandSonClass.vtab_size == sizeof(struct BarBadGrandSonVtable_stru)/sizeof(char) );
	assertTrue( BarBadGrandSonClass.init == BarBadGrandSon_initialize );
#ifndef OOC_NO_FINALIZE
	assertTrue( BarBadGrandSonClass.finz == BarBadGrandSon_finalize );
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
	assertTrue( ((BaseVtable)( FooClass.c.vtable ))->_class_register_next == & BarClass );
	assertTrue( ((BaseVtable)( BarClass.vtable ))->_class_register_prev == & FooClass );
	assertTrue( ((BaseVtable)( BarClass.c.vtable ))->_class_register_next == & BarSonClass );
	assertTrue( ((BaseVtable)( BarSonClass.c.vtable ))->_class_register_prev == & BarClass );
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
	assertTrue( ((BaseVtable)( FooClass.c.vtable ))->_class_register_next == & BarClass );
	assertTrue( ((BaseVtable)( BarClass.c.vtable ))->_class_register_prev == & FooClass );
	assertTrue( ((BaseVtable)( BarClass.c.vtable ))->_class_register_next == & BarSonClass );
	assertTrue( ((BaseVtable)( BarSonClass.c.vtable ))->_class_register_prev == & BarClass );
	assertNull( ((BaseVtable)( BarSonClass.c.vtable ))->_class_register_next );
				
	ooc_finalize_class( Bar );
	
	/* Ckeck if class initialization chain is correct */
	assertNotNull( ((BaseVtable)( FooClass.c.vtable ))->_class_register_prev );
	assertTrue( ((BaseVtable)( FooClass.c.vtable ))->_class_register_next == & BarSonClass );
	assertNull( ((BaseVtable)( BarClass.c.vtable ))->_class_register_prev );
	assertNull( ((BaseVtable)( BarClass.c.vtable ))->_class_register_next );
	assertTrue( ((BaseVtable)( BarSonClass.c.vtable ))->_class_register_prev == & FooClass );
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
	struct BarBadGrandSonObject bbgso;
	BarBadGrandSon bbgs = & bbgso;
	
	ooc_init_class( BarBadGrandSon );
	
	ooc_use( &bbgso, BarBadGrandSon, NULL );
	{
		ooc_manage( bbgs , (ooc_destroyer) ooc_release);
	
		BarBadGrandSonVirtual( bbgs )->BarSon.barson_virtual( (struct BarSonObject *) bbgs );
		BarBadGrandSonVirtual( bbgs )->BarSon.Bar.bar_virtual((struct BarObject *) bbgs );
		
		try {
			BarBadGrandSonVirtual( bbgs )->uninitialized_virtual();
			fail();
		}
		catch_any
			assertTrue( exception_get_error_code( exception ) == err_undefined_virtual );
		end_try;
		
		ooc_release( ooc_pass( (Object) bbgs ) );
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
test_lifecycle_static( void )
{
	struct FooLifeObject foolife1, foolife2;
	
		foolife_last_called = foolife_nothing;
	ooc_init_class( FooLife );
		assertTrue( foolife_last_called == foolife_init );
	ooc_use( &foolife1, FooLife, NULL );
		assertTrue( foolife_last_called == foolife_ctor );
	ooc_copy( &foolife2, (Object) &foolife1 );
		assertTrue( foolife_last_called == foolife_copy );
	ooc_release( (Object) &foolife1 );
		assertTrue( foolife_last_called == foolife_dtor );
#ifndef OOC_NO_FINALIZE
	ooc_finalize_class( FooLife );
		assertTrue( foolife_last_called == foolife_finz );
#endif
	
	/* prevent leaking foolife2 */
	ooc_init_class( FooLife );
	ooc_release( (Object) &foolife2 );
	ooc_finalize_class( FooLife );
}

static
void
test_use( void )
{
	struct FooObject foo;
	char testtext[] = "copyable test text";
	
	foo_use( &foo );
	assertNull( foo.text );
	assertZero( foo.data );
	ooc_release( (Object) &foo );
	
	foo_use_with_data( &foo, 100 );
	assertNull( foo.text );
	assertTrue( foo.data == 100 );
	ooc_release( (Object) &foo );

	foo_use_with_text( &foo, testtext );
 	assertZero( strcmp( foo.text, testtext ) );
	assertZero( foo.data );
	ooc_release( (Object) &foo );

	foo_use_with_data( &foo, 200 );
	assertNull( foo.text );
	assertTrue( foo.data == 200 );
	ooc_release( (Object) &foo );
}

/* Test constructors and inheritance
 */
 
static ROM char ctorcheck_params[] = "CtorCheck parameters";

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
	void * child_vtab;
EndOfClassMembers;

Virtuals( CtorCheck, Base )
EndOfVirtuals;

AllocateClass( CtorCheck, Base );

static	void	CtorCheck_initialize( Class this ) {}
#ifndef OOC_NO_FINALIZE
static	void	CtorCheck_finalize( Class this ) {}
#endif

static	void	CtorCheck_constructor( CtorCheck self, const void * params )
{
	assertTrue( ooc_isInitialized( CtorCheck ) );
	assertTrue( params == (const void *) ctorcheck_params );
	assertFalse( self->ctor_called );
	self->ctor_called = TRUE;
}

void * ctotcheckson_vtab = NULL;

static	void	CtorCheck_destructor( CtorCheck self, CtorCheckVtable vtab )
{
	assertNull( self->Base._vtab );
	assertTrue( vtab == ctotcheckson_vtab );
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

DeclareClass( CtorCheckSon, CtorCheck );

Virtuals( CtorCheckSon, CtorCheck )
EndOfVirtuals;

#define CCS_ARRAY_LEN 28

ClassMembers( CtorCheckSon, CtorCheck )
	int		array[ CCS_ARRAY_LEN ];
EndOfClassMembers;

AllocateClass( CtorCheckSon, CtorCheck );

static	void	CtorCheckSon_initialize( Class this )
{
	ctotcheckson_vtab = this->c.vtable;
}
#ifndef OOC_NO_FINALIZE
static	void	CtorCheckSon_finalize( Class this ) {}
#endif

static	void	CtorCheckSon_constructor( CtorCheckSon self, const void * params )
{	
	assertTrue( ooc_isInitialized( CtorCheckSon ) );
	assertTrue( params == (const void *) ctorcheck_params );
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
test_ctor_use( void )
{
	struct CtorCheckSonObject ccso;
	
	ooc_init_class(	CtorCheckSon );
	
	memset( &ccso, 0xAA, sizeof(struct CtorCheckSonObject)/sizeof(char) );
	
	ooc_use( &ccso, CtorCheckSon, (const void *) &ctorcheck_params );
	
	assertTrue( ooc_isInstanceOf( &ccso, CtorCheckSon ) );
	assertTrue( ccso.CtorCheck.ctor_called );
	assertTrue( ccso.CtorCheck.child_ctor_called );
	assertFalse( ccso.CtorCheck.dtor_called );
	assertFalse( ccso.CtorCheck.child_dtor_called );
	assertZero( ccso.CtorCheck.data );
	
	ooc_release( (Object) &ccso );
	assertTrue( ccso.CtorCheck.dtor_called );
	assertTrue( ccso.CtorCheck.child_dtor_called );

	ooc_release( (Object) &ccso );
	ooc_finalize_class( CtorCheckSon );
}

static
void
test_copy_static( void )
{
	static struct CtorCheckSonObject ccso, duplicate;
	
	ooc_init_class(	CtorCheckSon );
	
	ooc_use( &ccso, CtorCheckSon, (const void *) ctorcheck_params );
	memset( (GEN_PTR) &ccso.array, 0xAA, CCS_ARRAY_LEN );

	ccso.CtorCheck.copy_operation = OOC_COPY_DEFAULT;
	memset( (GEN_PTR) &duplicate, 0x55, sizeof(struct CtorCheckSonObject)/sizeof(char) );
	ooc_copy( &duplicate, (Object) &ccso );
	assertTrue( ooc_isInstanceOf( &duplicate, CtorCheckSon ) );
	assertZero( memcmp( (GEN_PTR) &ccso.array, (GEN_PTR) &duplicate.array, CCS_ARRAY_LEN ) );
	ooc_release( (Object) &duplicate );

	ccso.CtorCheck.copy_operation = OOC_COPY_DONE;
	memset( (GEN_PTR) &duplicate, 0x55, sizeof(struct CtorCheckSonObject)/sizeof(char) );
	ooc_copy( &duplicate, (Object) &ccso );
	assertTrue( duplicate.CtorCheck.copy_called );
	assertTrue( duplicate.CtorCheck.child_copy_called );
	ooc_release( (Object) &duplicate );

	ccso.CtorCheck.copy_operation = OOC_NO_COPY;
	try {
		memset( (GEN_PTR) &duplicate, 0x55, sizeof(struct CtorCheckSonObject)/sizeof(char) );
		ooc_copy( &duplicate, (Object) &ccso );
		fail();
	}
	catch_any {
		assertTrue( exception_get_error_code( exception ) == err_can_not_be_duplicated );
	}
	end_try;
	
	ooc_release( (Object) &ccso );
	ooc_finalize_class( CtorCheckSon );
}

static
void
test_ptr_read_and_null( void )
{
	void	*mem, *tmp;
	char	old[] = "data";

	mem = old;
	assertNotNull( mem );
	tmp = ooc_ptr_read_and_null( & mem );
	assertTrue( old == tmp );
	assertNull( mem );
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
	ooc_release( (Object) self->reference );
}
static	int		CircularCheck_copy( CircularCheck self, const CircularCheck from ) { return OOC_NO_COPY; }
static	void	circularcheck_set_reference( CircularCheck self, CircularCheck reference ) { self->reference = reference; }

static
void
test_release_circular( void )
{
	#define CCO_OBJECTS 5

	CircularCheck first, previous;
	struct CircularCheckObject cco[ CCO_OBJECTS ];
	int i;
	
	ooc_init_class( CircularCheck );
	
	ooc_use( &cco[0], CircularCheck, NULL );
	first = previous = &cco[0];
	
	for( i = 1; i < CCO_OBJECTS; i++ ) 
	{
		ooc_use( &cco[i], CircularCheck, NULL );
		circularcheck_set_reference( &cco[i], previous );
		previous = &cco[i];
	}
	circularcheck_set_reference( first, previous );
	
	ooc_release( (Object) first );
	
	ooc_finalize_class( CircularCheck );
}

typedef struct BarObject * Bar;
typedef struct BarObject * BarSon;
typedef struct BarObject * BarDaughter;

static
void
test_cast( void )
{
	static struct FooObject 			foo;
	static struct BarObject 			bar;
	static struct BarSonObject			barson;
	static struct BarDaughterObject		bardaughter;
	
	ooc_init_class( Foo );
	ooc_init_class( BarSon );
	ooc_init_class( BarDaughter );
	
	assertTrue( ooc_isClassOf( BarSon, Bar ) );
	assertTrue( ooc_isClassOf( BarDaughter, Bar ) );
	assertFalse( ooc_isClassOf( Bar, BarDaughter ) );
	assertFalse( ooc_isClassOf( Foo, Bar ) );

	foo_use( &foo );
	bar_use( &bar );
	barson_use( &barson );
	bardaughter_use( &bardaughter );

	assertTrue( ooc_isInstanceOf( &bar, Bar ) );
	assertTrue( ooc_isInstanceOf( &bardaughter, BarDaughter ) );
	assertTrue( ooc_isInstanceOf( &barson, BarSon ) );
	assertTrue( ooc_isInstanceOf( &bardaughter, Bar ) );
	assertTrue( ooc_isInstanceOf( &barson, Bar ) );
	assertFalse( ooc_isInstanceOf( &bar, BarSon ) );
	assertFalse( ooc_isInstanceOf( &bar, BarDaughter ) );
	assertFalse( ooc_isInstanceOf( &bar, Foo ) );
	
	assertTrue( ooc_get_type( (Object) &bar ) == &BarClass );
	assertTrue( ooc_get_type( (Object) &barson ) == &BarSonClass );
	assertTrue( ooc_get_type( (Object) &bardaughter ) == &BarDaughterClass );
	
	assertTrue( ooc_cast( &barson, Bar ) == (Bar) &barson );
	assertTrue( ooc_cast( &bardaughter, Bar ) == (Bar) &bardaughter );
	assertTrue( ooc_cast( &bar, Bar ) == (Bar) &bar );
	
	try {
		ooc_cast( &bar, BarSon );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;
	
	try {
		ooc_cast( &bardaughter, BarSon );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;
	
	try {
		ooc_cast( &bar, Foo );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;
	
	ooc_check_cast( &barson, &BarClass );
	ooc_check_cast( &bardaughter, &BarClass );
	ooc_check_cast( &bar, &BarClass );
	
	try {
		ooc_check_cast( &bar, &BarSonClass );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;
	
	try {
		ooc_check_cast( &bardaughter, &BarSonClass );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;
	
	try {
		ooc_check_cast( &bar, &FooClass );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	end_try;
	
	ooc_release( (Object) &foo );
	ooc_release( (Object) &bar );
	ooc_release( (Object) &barson );
	ooc_release( (Object) &bardaughter );
	
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
 
ROM struct TestCaseMethod methods[] =
{
	
	TEST(test_class_table),
	TEST(test_init),
	TEST(test_finalize),
	TEST(test_lifecycle_static),
	TEST(test_virtual_initialization),
	TEST(test_use),	
	TEST(test_ctor_use),
	TEST(test_copy_static),
	TEST(test_ptr_read_and_null),
	TEST(test_release_circular),
	TEST(test_cast),

	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */

/*typedef struct TestCaseObject * TestCase;
*/
 
TESTCASE_MAIN
{
	static struct OocTestObject ooctesto;
	int result;
	
	ooc_init_class( OocTest );
	ooc_use( &ooctesto, OocTest, (void *) &methods );
	result = testcase_run( (TestCase) &ooctesto );
	ooc_release( (Object) &ooctesto );
	ooc_finalize_all();

	abort();
}
