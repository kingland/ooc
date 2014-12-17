
/* This is a Flavour mixin implementation file
 */

#include "flavour.h"
#include "implement/flavour.h"

#include <ooc/exception.h>


AllocateMixin( Flavour );

/* Construction, destruction
 */

static
void
Flavour_constructor( Flavour flavour, FlavourData self )
{
	self->flavour = "flavourless";
}

static
void
Flavour_destructor( Flavour flavour, FlavourData self )
{
}

static void	Flavour_initialize() { }
static void	Flavour_finalize() { }

static
int
Flavour_copy( Flavour methods, FlavourData self, const FlavourData from )
{
	return OOC_COPY_DEFAULT;
}

/* Method implementations
 */

static
void
flavour_set( Object carrier, const char * flavour )
{
	FlavourData self = ooc_get_mixin_data( carrier, Flavour );

	self->flavour = flavour;
}

static
const char *
flavour_taste( Object carrier )
{
	FlavourData self = ooc_get_mixin_data( carrier, Flavour );

	return self->flavour;
}


/* Populating the interface's virtual table
 */

static
void
Flavour_populate( Flavour flavour )
{
	flavour->set	=	flavour_set;
	flavour->taste	=	flavour_taste;
}
