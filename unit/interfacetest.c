
/* This is a InterfaceTest class implementation file
 */

#include "../libs/testcase.h"

/** @class InterfaceTest
 *  @brief InterfaceTest class - brief description.
 * 	@see interfacetest.h
 */

/** @file interfacetest.h 
 *  @brief InterfaceTest class - brief description.
 * InterfaceTest is a .....
 * @note	This class is a final class, can not be inherited.
 * @note	Run as: valgrind --leak-check=yes --quiet ./interfacetest
 */ 

/* Test class hierarchy:
 *
 *              +---> MyInterface      ------> OtherInterface             MyMixin1
 *              |     ^         ^     /             ^                     ^
 *              |     |           \  /              |                     |
 *              |     Aclass       \/               |          Dclass     |
 *              |     ^            /\               |          ^          |
 *              |     |           /  \              |          |          |
 *  TestCase    |     Bclass------    --------------Eclass-----+----------+
 *  ^           |     ^                             ^
 *  |           |     |                             |
 *  InterfaceTest     Cclass                        Fclass
 */

#ifdef OOC_NO_FINALIZE
#define ooc_finalize_class( x )
#define ooc_finalize_all( )
#endif

DeclareInterface( MyInterface )

	void (* method1) ( Object );
	void (* method2) ( Object );

EndOfInterface;

DeclareInterface( OtherInterface )

	void (* otherMethod1) ( Object );
	void (* otherMethod2) ( Object );

EndOfInterface;

AllocateInterface( MyInterface );
AllocateInterface( OtherInterface );

/*-------------------------------------------------------*/

DeclareMixinInterface( MyMixin1 )

	void	(* mixin1_method1 )( Object );
	void	(* mixin1_method2 )( Object );

EndOfInterface;

MixinMembers( MyMixin1 )

	int		mixin1_data;

EndOfMixinMembers;

AllocateMixin( MyMixin1 );

static	int	mymixin1_initialize_called = 0;
#ifndef OOC_NO_FINALIZE
static	int	mymixin1_finalize_called = 0;
#endif

static void	MyMixin1_initialize() { mymixin1_initialize_called++; }
#ifndef OOC_NO_FINALIZE
static void	MyMixin1_finalize() { mymixin1_finalize_called++; }
#endif
static void	MyMixin1_constructor( MyMixin1 methods, MyMixin1Data self )
{
	self->mixin1_data = 1;
}

static int mymixin1_copy_mode = OOC_COPY_DEFAULT;

static int	MyMixin1_copy( MyMixin1 methods, MyMixin1Data self, const MyMixin1Data from )
{
	if( mymixin1_copy_mode == OOC_COPY_DONE )
	{
		self->mixin1_data = from->mixin1_data + 100;
	}
	return mymixin1_copy_mode;
}

static void	MyMixin1_destructor( MyMixin1 methods, MyMixin1Data self )
{
	self->mixin1_data = 99;
}

static void static_mixin1_method1( Object carrier )
{
	MyMixin1Data self = ooc_get_mixin_data( carrier, MyMixin1 );

	self->mixin1_data++;
}

static void MyMixin1_populate( MyMixin1 methods )
{
	methods->mixin1_method1 = static_mixin1_method1;
}

/*-------------------------------------------------------*/

DeclareClass( Aclass, Base );

ClassMembers( Aclass, Base )
EndOfClassMembers;

Virtuals( Aclass, Base )
	void (* dummy)( Aclass );
	Interface( MyInterface );
EndOfVirtuals;

InterfaceRegister( Aclass )
{
	AddInterface( Aclass, MyInterface ),
};

AllocateClassWithInterface( Aclass, Base );

static	void	Aclass_initialize( Class this ) {}
#ifndef OOC_NO_FINALIZE
static	void	Aclass_finalize( Class this ) {}
#endif
static	void	Aclass_constructor( Aclass self, const void * params )
{
	assert( ooc_isInitialized( Aclass ) );

	chain_constructor( Aclass, self, NULL );
}

static	void	Aclass_destructor( Aclass self, AclassVtable vtab ) {}
static	int		Aclass_copy( Aclass self, const Aclass from ) { return OOC_COPY_DEFAULT; }

/*-------------------------------------------------------*/

DeclareClass( Bclass, Aclass );

ClassMembers( Bclass, Aclass )
EndOfClassMembers;

Virtuals( Bclass, Aclass )
	void (* dummy) ( Bclass );
	Interface( OtherInterface );
EndOfVirtuals;

InterfaceRegister( Bclass )
{
	AddInterface( Bclass, OtherInterface )
};

AllocateClassWithInterface( Bclass, Aclass );

static	void	Bclass_initialize( Class this ) {}
#ifndef OOC_NO_FINALIZE
static	void	Bclass_finalize( Class this ) {}
#endif
static	void	Bclass_constructor( Bclass self, const void * params )
{
	assert( ooc_isInitialized( Bclass ) );

	chain_constructor( Bclass, self, NULL );
}

static	void	Bclass_destructor( Bclass self, BclassVtable vtab ) {}
static	int		Bclass_copy( Bclass self, const Bclass from ) { return OOC_COPY_DEFAULT; }

/*-------------------------------------------------------*/

DeclareClass( Cclass, Bclass );

ClassMembers( Cclass, Bclass )
EndOfClassMembers;

Virtuals( Cclass, Bclass )
EndOfVirtuals;

AllocateClass( Cclass, Bclass );

static	void	Cclass_initialize( Class this ) {}
#ifndef OOC_NO_FINALIZE
static	void	Cclass_finalize( Class this ) {}
#endif
static	void	Cclass_constructor( Cclass self, const void * params )
{
	assert( ooc_isInitialized( Cclass ) );

	chain_constructor( Cclass, self, NULL );
}

static	void	Cclass_destructor( Cclass self, CclassVtable vtab ) {}
static	int		Cclass_copy( Cclass self, const Cclass from ) { return OOC_COPY_DEFAULT; }

/*-------------------------------------------------------*/

DeclareClass( Dclass, Base );

ClassMembers( Dclass, Base )
EndOfClassMembers;

Virtuals( Dclass, Base )
EndOfVirtuals;

AllocateClass( Dclass, Base );

static	void	Dclass_initialize( Class this ) {}
#ifndef OOC_NO_FINALIZE
static	void	Dclass_finalize( Class this ) {}
#endif
static	void	Dclass_constructor( Dclass self, const void * params )
{
	assert( ooc_isInitialized( Dclass ) );

	chain_constructor( Dclass, self, NULL );
}

static	void	Dclass_destructor( Dclass self, DclassVtable vtab ) {}
static	int		Dclass_copy( Dclass self, const Dclass from ) { return OOC_COPY_DEFAULT; }

/*-------------------------------------------------------*/

DeclareClass( Eclass, Dclass );

ClassMembers( Eclass, Dclass )
	int		data;
	char	c;

	MixinData( MyMixin1 );
EndOfClassMembers;

Virtuals( Eclass, Dclass )
	void (* dummy)( Eclass );
	Interface( MyInterface );
	void (* dummy2)( Eclass );
	Interface( OtherInterface );
	void (* dummy3)( Eclass );
	Interface( MyMixin1 );
EndOfVirtuals;

InterfaceRegister( Eclass )
{
	AddInterface( Eclass, MyInterface ),
	AddMixin(     Eclass, MyMixin1 ),
	AddInterface( Eclass, OtherInterface )
};

AllocateClassWithInterface( Eclass, Dclass );

static	void	Eclass_initialize( Class this ) {}
#ifndef OOC_NO_FINALIZE
static	void	Eclass_finalize( Class this ) {}
#endif

static	void	Eclass_constructor( Eclass self, const void * params )
{
	assert( ooc_isInitialized( Eclass ) );

	chain_constructor( Eclass, self, NULL );
}

static	void	Eclass_destructor( Eclass self, EclassVtable vtab ) {}
static	int		Eclass_copy( Eclass self, const Eclass from ) { return OOC_COPY_DEFAULT; }

/*-------------------------------------------------------*/

DeclareClass( Fclass, Eclass );

ClassMembers( Fclass, Eclass )
EndOfClassMembers;

Virtuals( Fclass, Eclass )
EndOfVirtuals;

AllocateClass( Fclass, Eclass );

static	void	Fclass_initialize( Class this ) {}
#ifndef OOC_NO_FINALIZE
static	void	Fclass_finalize( Class this ) {}
#endif

static	void	Fclass_constructor( Fclass self, const void * params )
{
	assert( ooc_isInitialized( Fclass ) );

	chain_constructor( Fclass, self, NULL );
}

static	void	Fclass_destructor( Fclass self, FclassVtable vtab ) {}
static	int		Fclass_copy( Fclass self, const Fclass from ) { return OOC_COPY_DEFAULT; }

/*-------------------------------------------------------*/

DeclareClass( InterfaceTest, TestCase );

Virtuals( InterfaceTest, TestCase )

	Interface( MyInterface );

EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( InterfaceTest, TestCase )

	int		data;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

InterfaceRegister( InterfaceTest )
{
	AddInterface( InterfaceTest, MyInterface )
};

AllocateClassWithInterface( InterfaceTest, TestCase );


/* Class initializing
 */

static void interfacetest_before_class( InterfaceTest self );
static void interfacetest_before( InterfaceTest self );
static void interfacetest_after( InterfaceTest self );
static void interfacetest_after_class( InterfaceTest self );

static void dummy_method( Object );

static
void
InterfaceTest_initialize( Class this )
{
	InterfaceTestVtable vtab = & InterfaceTestVtableInstance;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) interfacetest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) interfacetest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) interfacetest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) interfacetest_after_class;

	vtab->MyInterface.method1 				= 	dummy_method;

	ooc_init_class( Exception );
	
	ooc_init_class( Aclass );
	ooc_init_class( Bclass );
	ooc_init_class( Cclass );
	ooc_init_class( Dclass );
	ooc_init_class( Eclass );
	ooc_init_class( Fclass );

	/* Allocate global resources here */
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static
void
InterfaceTest_finalize( Class this )
{
	/* Release global resources! */
}
#endif

/* Constructor
 */

static
void
InterfaceTest_constructor( InterfaceTest self, const void * params )
{
	assert( ooc_isInitialized( InterfaceTest ) );
	
	chain_constructor( InterfaceTest, self, params );
}

/* Destructor
 */

static
void
InterfaceTest_destructor( InterfaceTest self, InterfaceTestVtable vtab )
{
}

/* Copy constuctor
 */

static
int
InterfaceTest_copy( InterfaceTest self, const InterfaceTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
interfacetest_before_class( InterfaceTest self )
{
	return;
}

static
void
interfacetest_before( InterfaceTest self )
{
	return;
}

static
void
interfacetest_after( InterfaceTest self )
{
	return;
}

static
void
interfacetest_after_class( InterfaceTest self )
{
	return;
}


/*	=====================================================
	Test methods
 */

static
void
dummy_method( Object self )
{
	assertTrue( ooc_isInstanceOf( self, InterfaceTest ) );
	ooc_cast( self, InterfaceTest )->data++;
}

static
void
check_classtable( InterfaceTest self )
{
	assertTrue( InterfaceTestItable[0].id == & MyInterfaceID );
	assertNotZero( InterfaceTestItable[0].vtab_offset );
	assertTrue( InterfaceTestItable[0].vtab_offset == offsetof( struct InterfaceTestVtable_stru, MyInterface ) );
	assertTrue( ((char*) self->TestCase.Base._vtab) + InterfaceTestItable[0].vtab_offset
				== (char*) & InterfaceTestVirtual(self)->MyInterface );

	assertTrue( self->TestCase.Base._vtab->_class->itable == (Itable) &InterfaceTestItable );
	assertTrue( self->TestCase.Base._vtab->_class->itab_size == 1 );
}

static
void
check_init( InterfaceTest self )
{
	self->data = 0;
	InterfaceTestVirtual( self )->MyInterface.method1( (Object) self );
	assertTrue( self->data == 1 );

	try {
		InterfaceTestVirtual( self )->MyInterface.method2( (Object) self );
		fail();
		}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_undefined_virtual );
	end_try;
}

static
void
check_get_interface_I( InterfaceTest self )
{
	MyInterface myInterface = ooc_get_interface( self, MyInterface );

	assertTrue( myInterface == & InterfaceTestVirtual(self)->MyInterface );
}

static
void
check_get_interface_II( InterfaceTest self )
{
	OtherInterface otherInterface = ooc_get_interface( self, OtherInterface );

	assertNull( otherInterface );
}

static
void
check_get_interface_III( InterfaceTest self )
{
	Aclass	volatile a = NULL;
	Bclass	volatile b = NULL;
	Cclass	volatile c = NULL;

	try
		{
			MyInterface		myInterface;
			OtherInterface	otherInterface;

			a = ooc_new( Aclass, NULL );

			myInterface = ooc_get_interface( a, MyInterface );
			assertTrue( & AclassVirtual(a)->MyInterface == myInterface );
			assertNull( ooc_get_interface( a, OtherInterface ) );

			b = ooc_new( Bclass, NULL );

			myInterface = ooc_get_interface( b, MyInterface );
			assertTrue( & BclassVirtual(b)->Aclass.MyInterface == myInterface );
			otherInterface = ooc_get_interface( b, OtherInterface );
			assertTrue( & BclassVirtual(b)->OtherInterface == otherInterface );

			c = ooc_new( Cclass, NULL );

			myInterface = ooc_get_interface( c, MyInterface );
			assertTrue( & CclassVirtual(c)->Bclass.Aclass.MyInterface == myInterface );
			otherInterface = ooc_get_interface( c, OtherInterface );
			assertTrue( & CclassVirtual(c)->Bclass.OtherInterface == otherInterface );
		}
	finally
		{
			ooc_delete( (Object) a );
			ooc_delete( (Object) b );
			ooc_delete( (Object) c );
		}
	end_try;
}

static
void
check_get_interface_IV( InterfaceTest self )
{
	Dclass	volatile d = NULL;
	Eclass	volatile e = NULL;
	Fclass	volatile f = NULL;

	try
		{
			MyInterface		myInterface;
			OtherInterface	otherInterface;
			MyMixin1		myMixin1;

			d = ooc_new( Dclass, NULL );

			assertNull( ooc_get_interface( d, MyInterface ) );
			assertNull( ooc_get_interface( d, OtherInterface ) );
			assertNull( ooc_get_interface( d, MyMixin1 ) );

			e = ooc_new( Eclass, NULL );

			myInterface = ooc_get_interface( e, MyInterface );
			assertTrue( & EclassVirtual(e)->MyInterface == myInterface );
			otherInterface = ooc_get_interface( e, OtherInterface );
			assertTrue( & EclassVirtual(e)->OtherInterface == otherInterface );
			myMixin1 = ooc_get_interface( e, MyMixin1 );
			assertTrue( & EclassVirtual(e)->MyMixin1 == myMixin1 );

			f = ooc_new( Fclass, NULL );

			myInterface = ooc_get_interface( f, MyInterface );
			assertTrue( & FclassVirtual(f)->Eclass.MyInterface == myInterface );
			otherInterface = ooc_get_interface( f, OtherInterface );
			assertTrue( & FclassVirtual(f)->Eclass.OtherInterface == otherInterface );
			myMixin1 = ooc_get_interface( f, MyMixin1 );
			assertTrue( & FclassVirtual(f)->Eclass.MyMixin1 == myMixin1 );
		}
	finally
		{
			ooc_delete( (Object) d );
			ooc_delete( (Object) e );
			ooc_delete( (Object) f );
		}
	end_try;
}

static
void
check_get_interface_must_have( InterfaceTest self )
{
	Dclass	volatile d = NULL;
	Eclass	volatile e = NULL;
	Fclass	volatile f = NULL;

	try
		{
			MyInterface		myInterface;
			OtherInterface	otherInterface;
			MyMixin1		myMixin1;

			d = ooc_new( Dclass, NULL );

			try {
				ooc_get_interface_must_have( d, MyInterface );
				fail();
			}
			catch_any
				assertTrue( exception_get_error_code( exception ) == err_interface_not_implemented );
			end_try;

			try {
				ooc_get_interface_must_have( d, OtherInterface );
				fail();
			}
			catch_any
				assertTrue( exception_get_error_code( exception ) == err_interface_not_implemented );
			end_try;

			try {
				ooc_get_interface_must_have( d, MyMixin1 );
				fail();
			}
			catch_any
				assertTrue( exception_get_error_code( exception ) == err_interface_not_implemented );
			end_try;

			e = ooc_new( Eclass, NULL );

			myInterface = ooc_get_interface_must_have( e, MyInterface );
			assertTrue( & EclassVirtual(e)->MyInterface == myInterface );
			otherInterface = ooc_get_interface_must_have( e, OtherInterface );
			assertTrue( & EclassVirtual(e)->OtherInterface == otherInterface );
			myMixin1 = ooc_get_interface_must_have( e, MyMixin1 );
			assertTrue( & EclassVirtual(e)->MyMixin1 == myMixin1 );

			f = ooc_new( Fclass, NULL );

			myInterface = ooc_get_interface_must_have( f, MyInterface );
			assertTrue( & FclassVirtual(f)->Eclass.MyInterface == myInterface );
			otherInterface = ooc_get_interface_must_have( f, OtherInterface );
			assertTrue( & FclassVirtual(f)->Eclass.OtherInterface == otherInterface );
			myMixin1 = ooc_get_interface_must_have( f, MyMixin1 );
			assertTrue( & FclassVirtual(f)->Eclass.MyMixin1 == myMixin1 );
		}
	finally
		{
			ooc_delete( (Object) d );
			ooc_delete( (Object) e );
			ooc_delete( (Object) f );
		}
	end_try;
}

static
void
check_mixin_initialization( InterfaceTest self )
{
	assertTrue( mymixin1_initialize_called == 1 );
	assertTrue( EclassVtableInstance.MyMixin1.mixin1_method1 == & static_mixin1_method1 );
	assertTrue( FclassVtableInstance.Eclass.MyMixin1.mixin1_method1 == & static_mixin1_method1 );

	ooc_init_class( Fclass );
	ooc_init_class( Eclass );
	assertTrue( mymixin1_initialize_called == 1 );
}

#ifndef OOC_NO_FINALIZE
static
void
check_mixin_finalization( InterfaceTest self )
{
	int finz_called;

	assertTrue( mymixin1_finalize_called == 0 );
	ooc_finalize_class( Eclass );
	assertTrue( mymixin1_finalize_called == 0 );

	ooc_finalize_all();
	finz_called = mymixin1_finalize_called;
	ooc_init_class( InterfaceTest );
	assertTrue( finz_called == 1 );
}
#endif

static
void
check_mixin_construction( InterfaceTest self )
{
	Fclass f;

	f = ooc_new( Fclass, NULL );
	assertTrue( f->Eclass.MyMixin1.mixin1_data == 1 );
	ooc_release( (Object) f );
	assertTrue( f->Eclass.MyMixin1.mixin1_data == 99 );
	ooc_free( f );
}

static
void
check_mixin_method( InterfaceTest self )
{
	Fclass volatile f = NULL;

	try
	{
		MyMixin1 mixin;

		f = ooc_new( Fclass, NULL );
		assertTrue( f->Eclass.MyMixin1.mixin1_data == 1 );
		mixin = ooc_get_interface_must_have( f, MyMixin1 );

		mixin->mixin1_method1( (Object) f );
		assertTrue( f->Eclass.MyMixin1.mixin1_data == 2 );
		mixin->mixin1_method1( (Object) f );
		assertTrue( f->Eclass.MyMixin1.mixin1_data == 3 );
	}
	finally
	{
		ooc_delete( (Object) f );
	}
	end_try;
}

static
void
check_mixin_copy_default( InterfaceTest self )
{
	Fclass	f1, f2;

	mymixin1_copy_mode = OOC_COPY_DEFAULT;
	f1 = ooc_new( Fclass, NULL );
	{
		ooc_manage_object( f1 );
		f1->Eclass.data = 200;
		f1->Eclass.MyMixin1.mixin1_data = 300;
		f2 = (Fclass) ooc_duplicate( (Object) f1 );
		assertTrue( f2->Eclass.data == 200 );
		assertTrue( f2->Eclass.MyMixin1.mixin1_data == 300 );
		ooc_delete( (Object) f2 );
		ooc_delete( ooc_pass( (Object) f1 ) );
	}
}

static
void
check_mixin_copy_done( InterfaceTest self )
{
	Fclass	f1, f2;

	mymixin1_copy_mode = OOC_COPY_DONE;
	f1 = ooc_new( Fclass, NULL );
	{
		ooc_manage_object( f1 );
		f1->Eclass.data = 200;
		f1->Eclass.MyMixin1.mixin1_data = 300;
		f2 = (Fclass) ooc_duplicate( (Object) f1 );
		assertTrue( f2->Eclass.data == 200 );
		assertTrue( f2->Eclass.MyMixin1.mixin1_data == 300 + 100 );
		ooc_delete( (Object) f2 );
		ooc_delete( ooc_pass( (Object) f1 ) );
	}
}

static
void
check_mixin_no_copy( InterfaceTest self )
{
	Fclass	volatile f1 = NULL;

	mymixin1_copy_mode = OOC_NO_COPY;
	f1 = ooc_new( Fclass, NULL );

	try {
		ooc_duplicate( (Object) f1 );
		fail();
	}
	catch_any {
		assertTrue( exception_get_error_code( exception ) == err_can_not_be_duplicated );
	}
	finally {
		ooc_delete( (Object) f1 );
	}
	end_try;
}


/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
static ROM_ALLOC struct TestCaseMethod methods[] =
{
	
	TEST(check_classtable),
	TEST(check_init),
	TEST(check_get_interface_I),
	TEST(check_get_interface_II),
	TEST(check_get_interface_III),
	TEST(check_get_interface_IV),
	TEST(check_get_interface_must_have),
	TEST(check_mixin_initialization),
#ifndef OOC_NO_FINALIZE
	TEST(check_mixin_finalization),
#endif
	TEST(check_mixin_construction),
	TEST(check_mixin_method),
	TEST(check_mixin_copy_default),
	TEST(check_mixin_copy_done),
	TEST(check_mixin_no_copy),
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
TESTCASE_MAIN
{
	InterfaceTest interfacetest;
	int result;
	
	ooc_init_class( InterfaceTest );
	interfacetest = ooc_new( InterfaceTest, &methods );
	result = testcase_run((TestCase)interfacetest);
	ooc_delete( (Object) interfacetest );
	ooc_finalize_all();
	return result;
}
