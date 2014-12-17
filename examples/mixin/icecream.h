
#ifndef ICECREAM_H
#define ICECREAM_H

#include <ooc/ooc.h>

#include "tonic.h"
#include "flavour.h"

DeclareClass( IceCream, Tonic );

IceCream icecream_new( const char * flavour );

/* Virtual function definitions
 */

Virtuals( IceCream, Tonic )

	Interface( Flavour );

EndOfVirtuals;

#endif  /* ICECREAM_H */
