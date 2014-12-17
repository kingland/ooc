
/* This is a Sample class implementation file
 */

#include <ooc/testcase.h>

#ifdef OOC_NO_FINALIZE
#define ooc_finalize_class( x )
#define ooc_finalize_all( )
#endif


/** @class Sample
 *  @brief Sample class - brief description.
 * 	@see sample.h
 */

/** @file sample.h 
 *  @brief Sample class - brief description.
 * Sample is a .....
 * @note	This class is a final class, can not be inherited.
 * @note	Run as: valgrind --leak-check=yes --quiet ./sample
 */ 

DeclareClass( Sample, TestCase );

Virtuals( Sample, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( Sample, TestCase )

	int		data;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( Sample, TestCase );


/* Class initializing
 */

static void sample_before_class( Sample self );
static void sample_before( Sample self );
static void sample_after( Sample self );
static void sample_after_class( Sample self );

static
void
Sample_initialize( Class this )
{
	SampleVtable vtab = & SampleVtableInstance;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) sample_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) sample_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) sample_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) sample_after_class;

	ooc_init_class( Exception );
	
	ooc_init_class( !!! Classes that are USED by Sample !!! ); Remove if you initialize it other place!
	
	/* Allocate global resources here */
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE

static
void
Sample_finalize( Class this )
{
	/* Release global resources! */
}

#endif

/* Constructor
 */

static
void
Sample_constructor( Sample self, const void * params )
{
	assert( ooc_isInitialized( Sample ) );
	
	chain_constructor( Sample, self, params );
}

/* Destructor
 */

static
void
Sample_destructor( Sample self, SampleVtable vtab )
{
}

/* Copy constuctor
 */

static
int
Sample_copy( Sample self, const Sample from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
sample_before_class( Sample self )
{
	return;
}

static
void
sample_before( Sample self )
{
	return;
}

static
void
sample_after( Sample self )
{
	return;
}

static
void
sample_after_class( Sample self )
{
	return;
}


/*	=====================================================
	Test methods
 */

void
static
sample_method1( Sample self )
{
	failMsg("Intentionally failed");
}

/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
static ROM_ALLOC struct TestCaseMethod methods[] =
{
	
	TEST(sample_method1),
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
TESTCASE_MAIN
{
	Sample sample;
	int result;
	
	ooc_init_class( Sample );
	sample = ooc_new( Sample, &methods );
	result = testcase_run((TestCase)sample);
	ooc_delete( (Object) sample );
	ooc_finalize_all();
	return result;
}
