
/* This is a Sample interface (light-weight mixin) implementation file
 */

#include "sample.h"
#include "implement/sample.h"

#include <ooc/exception.h>

/** @class Sample
 *  @brief Sample interface - brief description.
 * 	@see sample.h
 */

/** @file sample.h
 *  @brief Sample interface - brief description.
 * Sample is a .....
 */

AllocateMixin( Sample );

/* Initialization, finalization
 */

static
void
Sample_initialize()
{
}

static
void
Sample_finalize()
{
}


/* Construction, destruction
 */

static
void
Sample_constructor( Sample sample, SampleData self )
{
	/* Build up your SampleDataFields.
	 * This method is called ones for every Object, and every field is zeroed at entry.
	 */
	self->data = ?
}

static
int
Sample_copy( Sample sample, SampleData self, const SampleData from )
{
	return OOC_COPY_DEFAULT;
	return OOC_COPY_DONE;
	return OOC_NO_COPY;
}

static
void
Sample_destructor( Sample sample, SampleData self )
{

}


/* Method implementations
 */

static
void
sample_my_method1( Object carrier, .. other params .. )
{
	SampleData self = ooc_get_mixin_data( carrier, Sample );

	self->data = ...
}


/* Populating the interface's virtual table
 */

void
Sample_populate( Sample sample )s
{
	// Fill the Sample methods table with the methods.

	sample->my_method1	=	sample_my_method1;
	...
}
