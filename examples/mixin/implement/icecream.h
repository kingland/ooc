
#ifndef IMPL_ICECREAM_H
#define IMPL_ICECREAM_H

#ifndef ICECREAM_H
#error Do #include "icecream.h" right before doing #include "implement/icecream.h"
#endif

#ifndef NO_RELATIVE_INCLUDE
#include "tonic.h"
#include "flavour.h"
#else
#include "implement/tonic.h"
#include "implement/flavour.h"
#endif


ClassMembers( IceCream, Tonic )

	int		scoop;

	MixinData( Flavour );

EndOfClassMembers;


#endif /* IMPL_ICECREAM_H */
