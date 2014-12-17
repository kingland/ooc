
#ifndef IMPL_BOOKLET_H
#define IMPL_BOOKLET_H

#ifndef BOOKLET_H
#error Do #include "booklet.h" right before doing #include "implement/booklet.h"
#endif

#include <ooc/vector.h>

ClassMembers( Booklet, Base )

	const char *	owner;
	const char *	date;
	Vector			notes;

EndOfClassMembers;


#endif /* IMPL_BOOKLET_H */
