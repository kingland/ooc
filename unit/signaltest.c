
/* This is a SignalTest class implementation file
 */

#include "../libs/testcase.h"

#include "../libs/signal.h"

/** @class SignalTest
 *  @brief SignalTest class - brief description.
 * 	@see signaltest.h
 */

/** @file signaltest.h 
 *  @brief SignalTest class - brief description.
 * SignalTest is a .....
 * @note	This class is a final class, can not be inherited.
 * @note	Run as: valgrind --leak-check=yes --quiet ./signaltest
 */ 

#ifdef OOC_NO_FINALIZE
#define ooc_finalize_class( x )
#define ooc_finalize_all( )
#endif

/*	=====================================================
	Test helper classes
 */

DeclareClass( Source, Base );

Virtuals( Source, Base )

EndOfVirtuals;

ClassMembers( Source, Base )

	int		index;
	
	int		counter_max;
	int		counter;
	
	#ifndef OOC_NO_THREADS
	ooc_Mutex	modify;
	#endif

	Signal	on_counter_reached;
	
EndOfClassMembers;

AllocateClass( Source, Base );

static
void
Source_initialize( Class this )
{
	ooc_init_class( Signal );
}

#ifndef OOC_NO_FINALIZE

static
void
Source_finalize( Class this )
{
}
#endif

static
void
Source_constructor( Source self, const void * params )
{
	assert( ooc_isInitialized( Source ) );
	
	chain_constructor( Source, self, NULL );
	
	ooc_mutex_init( self->modify );
}

static
void
Source_destructor( Source self, SourceVtable vtab )
{
	signal_destroy_notify( (Object) self );

	ooc_mutex_release( self->modify );
}

static
int
Source_copy( Source self, const Source from )
{
	return OOC_NO_COPY;
}

static
Source
source_new( int index, int period )
{
	Source self;
	
	ooc_init_class( Source );
	
	self = ooc_new( Source, NULL );
	
	self->index = index;
	self->counter_max = period;
	
	return self;
}

static
Signal *
source_counter_reached_signal( void * self )
{
	return & ooc_cast( self, Source )->on_counter_reached;
}

static char * signal_param = "Signal parameter";

static
void
source_count( Source self )
{
	int must_emit = FALSE;
	
	assert( ooc_isInstanceOf( self, Source ) );
	
	ooc_lock( self->modify );
	if( (++ self->counter) == self->counter_max ) {
		self->counter = 0;
		must_emit = TRUE;
		}
	ooc_unlock( self->modify );
	
	if( must_emit )
		signal_emit( self->on_counter_reached, signal_param, NULL );
}

static
void
source_count_sync( Source self )
{
	int must_emit = FALSE;
	
	assert( ooc_isInstanceOf( self, Source ) );
	
	ooc_lock( self->modify );
	if( (++ self->counter) == self->counter_max ) {
		self->counter = 0;
		must_emit = TRUE;
		}
	ooc_unlock( self->modify );
	
	if( must_emit )
		signal_emit_sync( self->on_counter_reached, signal_param, NULL );
}

#define MAX_SOURCES 10

/* This is a Listener class implementation
 */

DeclareClass( Listener, Base );

Virtuals( Listener, Base )

EndOfVirtuals;

ClassMembers( Listener, Base )

	int		source_fired_count[ MAX_SOURCES ];
	
	#ifndef OOC_NO_THREADS
	ooc_Mutex	modify;
	#endif
	
EndOfClassMembers;

AllocateClass( Listener, Base );

static
void
Listener_initialize( Class this )
{
	ooc_init_class( Signal );
}

#ifndef OOC_NO_FINALIZE
static
void
Listener_finalize( Class this )
{
}
#endif

static
void
Listener_constructor( Listener self, const void * params )
{
	assert( ooc_isInitialized( Listener ) );
	
	chain_constructor( Listener, self, NULL );
	
	ooc_mutex_init( self->modify );
}

static
void
Listener_destructor( Listener self, ListenerVtable vtab )
{
	signal_destroy_notify( (Object) self );	
	
	ooc_mutex_release( self->modify );
}

static
int
Listener_copy( Listener self, const Listener from )
{
	/* makes the default object copying (bit-by-bit) */
	return OOC_COPY_DEFAULT;
}

static
Listener
listener_new( void )
{
	ooc_init_class( Listener );
			
	return ooc_new( Listener, NULL );
}


static
void
listener_counter_reached( Listener self, Source source, const char * param )
{
	assertTrue( ooc_isInstanceOf( self, Listener ) );
	assertTrue( ooc_isInstanceOf( source, Source ) );
	assertTrue( source->index < MAX_SOURCES && source->index >= 0 );

	assertTrue( param == signal_param );
	
	ooc_lock( self->modify );
	++ ( self->source_fired_count[ source->index ] );
	ooc_unlock( self->modify );
}

/*	=====================================================
	SignalTest implementation
 */
 
DeclareClass( SignalTest, TestCase );

Virtuals( SignalTest, TestCase )
EndOfVirtuals;

ClassMembers( SignalTest, TestCase )

	Source		source1;
	Source		source2;
	
	Listener	listener1;
	Listener	listener2;

EndOfClassMembers;

AllocateClass( SignalTest, TestCase );


/* Class initializing
 */

static void signaltest_before_class( SignalTest self );
static void signaltest_before( SignalTest self );
static void signaltest_after( SignalTest self );
static void signaltest_after_class( SignalTest self );

static
void
SignalTest_initialize( Class this )
{
	SignalTestVtable vtab = & SignalTestVtableInstance;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) signaltest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) signaltest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) signaltest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) signaltest_after_class;

	ooc_init_class( Exception );
	
	/* Allocate global resources here */
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static
void
SignalTest_finalize( Class this )
{
	/* Release global resources! */
}
#endif

/* Constructor
 */

static
void
SignalTest_constructor( SignalTest self, const void * params )
{
	assert( ooc_isInitialized( SignalTest ) );
	
	chain_constructor( SignalTest, self, params );
}

/* Destructor
 */

static
void
SignalTest_destructor( SignalTest self, SignalTestVtable vtab )
{
}

/* Copy constuctor
 */

static
int
SignalTest_copy( SignalTest self, const SignalTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
signaltest_before_class( SignalTest self )
{
	return;
}

#define SOURCE1_PERIOD 17
#define SOURCE2_PERIOD 27

static
void
signaltest_before( SignalTest self )
{
	ooc_init_class( Signal );

	self->source1 = source_new( 0, SOURCE1_PERIOD );
	self->source2 = source_new( 1, SOURCE2_PERIOD );
	self->listener1 = listener_new();
	self->listener2 = listener_new();
}

static
void
signaltest_after( SignalTest self )
{
	ooc_delete_and_null( (Object*) & self->source1 );
	ooc_delete_and_null( (Object*) & self->source2 );
	ooc_delete_and_null( (Object*) & self->listener1 );
	ooc_delete_and_null( (Object*) & self->listener2 );

	ooc_finalize_class( Signal );
}

static
void
signaltest_after_class( SignalTest self )
{
	return;
}

/*	=====================================================
	Test methods
 */

void
static
signaltest_bad_connect( SignalTest self )
{
	try {
		signal_connect ( self->listener1 /* bad, ought to be a Source */, 
						source_counter_reached_signal,
						self->listener1,
						(SignalHandler) listener_counter_reached );
		fail();
	}
	catch_any {
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	}
	end_try;
	
}

void
static
signaltest_2sources_1listener( SignalTest self )
{
	#define CYCLE 1000
	
	int i;
	
	signal_connect ( self->source1, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source2, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	
	#pragma omp parallel for private(i)
	for( i=0; i<CYCLE; i++ ) {
		source_count( self->source1 );
		source_count( self->source2 );
		if( i % 7  == 0 )
			signal_process_signals();
		}
	signal_process_signals();
	
	assertTrue( self->listener1->source_fired_count[0] == CYCLE / SOURCE1_PERIOD );
	assertTrue( self->listener1->source_fired_count[1] == CYCLE / SOURCE2_PERIOD );

	for( i = 2; i<MAX_SOURCES; i++ )	
		assertTrue( self->listener1->source_fired_count[i] == 0 );
		
	#undef CYCLE
}

void
static
signaltest_1source_2listeners( SignalTest self )
{
	#define CYCLE 1000
	
	int i;
	
	signal_connect ( self->source1, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source1, source_counter_reached_signal, self->listener2, (SignalHandler) listener_counter_reached );
	
	#pragma omp parallel for private(i)
	for( i=0; i<CYCLE; i++ ) {
		source_count( self->source1 );
		if( i % 7  == 0 )
			signal_process_signals();
		}
	signal_process_signals();
	
	assertTrue( self->listener1->source_fired_count[0] == CYCLE / SOURCE1_PERIOD );
	for( i = 1; i<MAX_SOURCES; i++ )	
		assertTrue( self->listener1->source_fired_count[i] == 0 );
		
	assertTrue( self->listener2->source_fired_count[0] == CYCLE / SOURCE1_PERIOD );
	for( i = 1; i<MAX_SOURCES; i++ )	
		assertTrue( self->listener2->source_fired_count[i] == 0 );
		
	#undef CYCLE
}

void
static
signaltest_2sources_2listeners( SignalTest self )
{
	#define CYCLE 1000
	
	int i;
	
	signal_connect ( self->source1, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source1, source_counter_reached_signal, self->listener2, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source2, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source2, source_counter_reached_signal, self->listener2, (SignalHandler) listener_counter_reached );
	
	#pragma omp parallel for private(i)
	for( i=0; i<CYCLE; i++ ) {
		source_count( self->source1 );
		source_count( self->source2 );
		if( i % 7  == 0 )
			signal_process_signals();
		}
	signal_process_signals();
	
	assertTrue( self->listener1->source_fired_count[0] == CYCLE / SOURCE1_PERIOD );
	assertTrue( self->listener1->source_fired_count[1] == CYCLE / SOURCE2_PERIOD );
	for( i = 2; i<MAX_SOURCES; i++ )	
		assertTrue( self->listener1->source_fired_count[i] == 0 );
		
	assertTrue( self->listener2->source_fired_count[0] == CYCLE / SOURCE1_PERIOD );
	assertTrue( self->listener2->source_fired_count[1] == CYCLE / SOURCE2_PERIOD );
	for( i = 2; i<MAX_SOURCES; i++ )	
		assertTrue( self->listener2->source_fired_count[i] == 0 );
		
	#undef CYCLE
}

void
static
signaltest_2sources_1listener_sync( SignalTest self )
{
	#define CYCLE 1000
	
	int i;
	
	signal_connect ( self->source1, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source2, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	
	#pragma omp parallel for private(i)
	for( i=0; i<CYCLE; i++ ) {
		source_count_sync( self->source1 );
		source_count_sync( self->source2 );
		}

	assertTrue( self->listener1->source_fired_count[0] == CYCLE / SOURCE1_PERIOD );
	assertTrue( self->listener1->source_fired_count[1] == CYCLE / SOURCE2_PERIOD );

	for( i = 2; i<MAX_SOURCES; i++ )	
		assertTrue( self->listener1->source_fired_count[i] == 0 );
		
	#undef CYCLE
}

void
static
signaltest_1source_2listeners_sync( SignalTest self )
{
	#define CYCLE 1000
	
	int i;
	
	signal_connect ( self->source1, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source1, source_counter_reached_signal, self->listener2, (SignalHandler) listener_counter_reached );
	
	#pragma omp parallel for private(i)
	for( i=0; i<CYCLE; i++ ) {
		source_count_sync( self->source1 );
		}
	
	assertTrue( self->listener1->source_fired_count[0] == CYCLE / SOURCE1_PERIOD );
	for( i = 1; i<MAX_SOURCES; i++ )	
		assertTrue( self->listener1->source_fired_count[i] == 0 );
		
	assertTrue( self->listener2->source_fired_count[0] == CYCLE / SOURCE1_PERIOD );
	for( i = 1; i<MAX_SOURCES; i++ )	
		assertTrue( self->listener2->source_fired_count[i] == 0 );
		
	#undef CYCLE
}

void
static
signaltest_2sources_2listeners_sync( SignalTest self )
{
	#define CYCLE 1000
	
	int i;
	
	signal_connect ( self->source1, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source1, source_counter_reached_signal, self->listener2, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source2, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source2, source_counter_reached_signal, self->listener2, (SignalHandler) listener_counter_reached );
	
	#pragma omp parallel for private(i)
	for( i=0; i<CYCLE; i++ ) {
		source_count_sync( self->source1 );
		source_count_sync( self->source2 );
		}
		
	assertTrue( self->listener1->source_fired_count[0] == CYCLE / SOURCE1_PERIOD );
	assertTrue( self->listener1->source_fired_count[1] == CYCLE / SOURCE2_PERIOD );
	for( i = 2; i<MAX_SOURCES; i++ )	
		assertTrue( self->listener1->source_fired_count[i] == 0 );
		
	assertTrue( self->listener2->source_fired_count[0] == CYCLE / SOURCE1_PERIOD );
	assertTrue( self->listener2->source_fired_count[1] == CYCLE / SOURCE2_PERIOD );
	for( i = 2; i<MAX_SOURCES; i++ )	
		assertTrue( self->listener2->source_fired_count[i] == 0 );
		
	#undef CYCLE
}


void
static
signaltest_disconnect_I( SignalTest self )
{
	#define CYCLE 1000
	
	int i;
	
	signal_connect ( self->source1, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source1, source_counter_reached_signal, self->listener2, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source2, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source2, source_counter_reached_signal, self->listener2, (SignalHandler) listener_counter_reached );
	
	#pragma omp parallel for private(i)
	for( i=0; i<CYCLE; i++ ) {
		source_count_sync( self->source1 );
		source_count_sync( self->source2 );
		}
		
	signal_disconnect ( self->source2, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	signal_disconnect ( self->source2, source_counter_reached_signal, self->listener2, (SignalHandler) listener_counter_reached );
	
	#pragma omp parallel for private(i)
	for( i=0; i<CYCLE; i++ ) {
		source_count_sync( self->source1 );
		source_count_sync( self->source2 );
		}
		
	assertTrue( self->listener1->source_fired_count[0] == CYCLE * 2 / SOURCE1_PERIOD );
	assertTrue( self->listener1->source_fired_count[1] == CYCLE / SOURCE2_PERIOD );
	for( i = 2; i<MAX_SOURCES; i++ )	
		assertTrue( self->listener1->source_fired_count[i] == 0 );
		
	assertTrue( self->listener2->source_fired_count[0] == CYCLE  * 2 / SOURCE1_PERIOD );
	assertTrue( self->listener2->source_fired_count[1] == CYCLE / SOURCE2_PERIOD );
	for( i = 2; i<MAX_SOURCES; i++ )	
		assertTrue( self->listener2->source_fired_count[i] == 0 );
		
	#undef CYCLE
}

void
static
signaltest_disconnect_II( SignalTest self )
{
	#define CYCLE 1000
	
	int i;
	
	signal_connect ( self->source1, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source1, source_counter_reached_signal, self->listener2, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source2, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source2, source_counter_reached_signal, self->listener2, (SignalHandler) listener_counter_reached );
	
	#pragma omp parallel for private(i)
	for( i=0; i<CYCLE; i++ ) {
		source_count_sync( self->source1 );
		source_count_sync( self->source2 );
		}
		
	signal_disconnect ( self->source1, source_counter_reached_signal, self->listener2, (SignalHandler) listener_counter_reached );
	signal_disconnect ( self->source2, source_counter_reached_signal, self->listener2, (SignalHandler) listener_counter_reached );
	
	#pragma omp parallel for private(i)
	for( i=0; i<CYCLE; i++ ) {
		source_count_sync( self->source1 );
		source_count_sync( self->source2 );
		}
		
	assertTrue( self->listener1->source_fired_count[0] == CYCLE * 2 / SOURCE1_PERIOD );
	assertTrue( self->listener1->source_fired_count[1] == CYCLE * 2 / SOURCE2_PERIOD );
	for( i = 2; i<MAX_SOURCES; i++ )	
		assertTrue( self->listener1->source_fired_count[i] == 0 );
		
	assertTrue( self->listener2->source_fired_count[0] == CYCLE / SOURCE1_PERIOD );
	assertTrue( self->listener2->source_fired_count[1] == CYCLE / SOURCE2_PERIOD );
	for( i = 2; i<MAX_SOURCES; i++ )	
		assertTrue( self->listener2->source_fired_count[i] == 0 );
		
	#undef CYCLE
}

void
static
signaltest_process_parallel( SignalTest self )
{
	#define CYCLE 1000
	
	int i;
	
	signal_connect ( self->source1, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	signal_connect ( self->source2, source_counter_reached_signal, self->listener1, (SignalHandler) listener_counter_reached );
	
	#pragma omp parallel for private(i)
	for( i=0; i<CYCLE; i++ ) {
		source_count( self->source1 );
		source_count( self->source2 );
		}

	#pragma omp parallel
	while( signal_process_next() );
		
	assertTrue( self->listener1->source_fired_count[0] == CYCLE / SOURCE1_PERIOD );
	assertTrue( self->listener1->source_fired_count[1] == CYCLE / SOURCE2_PERIOD );

	for( i = 2; i<MAX_SOURCES; i++ )	
		assertTrue( self->listener1->source_fired_count[i] == 0 );
		
	#undef CYCLE
}

/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
ROM_ALLOC
struct TestCaseMethod methods[] =
{
	TEST(signaltest_bad_connect),
	
	TEST(signaltest_2sources_1listener),
	TEST(signaltest_1source_2listeners),
	TEST(signaltest_2sources_2listeners),

	TEST(signaltest_2sources_1listener_sync),
	TEST(signaltest_1source_2listeners_sync),
	TEST(signaltest_2sources_2listeners_sync),

	TEST(signaltest_disconnect_I),
	TEST(signaltest_disconnect_II),

	TEST(signaltest_process_parallel),

	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
TESTCASE_MAIN
{
	SignalTest signaltest;
	int result;
	
	ooc_init_class( SignalTest );
	signaltest = ooc_new( SignalTest, &methods );
	result = testcase_run((TestCase)signaltest);
	ooc_delete( (Object) signaltest );
	ooc_finalize_all();
	return result;
}
