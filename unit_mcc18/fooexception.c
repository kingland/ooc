/* Inherited Exception for tests
 */
 
#include "fooexception.h"

ClassMembers( FooException, Exception )
EndOfClassMembers;

Virtuals( FooException, Exception )
EndOfVirtuals;

AllocateClass( FooException, Exception );

static	void	FooException_initialize( Class this ) {}
#ifndef OOC_NO_FINALIZE
static	void	FooException_finalize( Class this ) {}
#endif

static	void	FooException_constructor( FooException self, const void * params )
{ 
	assert( ooc_isInitialized( FooException ) );
	chain_constructor( FooException, self, NULL );
	if( params )
		self->Exception.user_code = * ( (int*) params );
}

static	void	FooException_destructor( FooException self, FooExceptionVtable vtab ) {}
static	int		FooException_copy( FooException self, const FooException from ) { return OOC_COPY_DEFAULT; }

typedef struct ExceptionObject * Exception;
Exception
foo_exception_new( int fooerror_code ) 
{ 
	static struct FooExceptionObject fooexception;

	ooc_init_class( FooException );

	ooc_use( &fooexception, FooException, (void*) &fooerror_code );

	return (Exception) & fooexception;
}

