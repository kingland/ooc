#ifndef FOOEXCEPTION_H_
#define FOOEXCEPTION_H_

#include "../libs/exception.h"
#include "../libs/implement/exception.h"

/* Inherited Exception for tests
 */

DeclareClass( FooException, Exception );

Exception foo_exception_new( int fooerror_code );

#endif /*FOOEXCEPTION_H_*/
