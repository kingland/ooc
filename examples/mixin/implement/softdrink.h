
#ifndef IMPL_SOFTDRINK_H
#define IMPL_SOFTDRINK_H

#ifndef SOFTDRINK_H
#error Do #include "softdrink.h" right before doing #include "implement/softdrink.h"
#endif

#ifndef NO_RELATIVE_INCLUDE
#include "tonic.h"
#else
#include "implement/tonic.h"
#endif


ClassMembers( SoftDrink, Tonic )

	int		size;

EndOfClassMembers;


#endif /* IMPL_SOFTDRINK_H */
