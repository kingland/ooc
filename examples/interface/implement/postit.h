
#ifndef IMPL_POSTIT_H
#define IMPL_POSTIT_H

#ifndef POSTIT_H
#error Do #include "postit.h" right before doing #include "implement/postit.h"
#endif

#ifndef NO_RELATIVE_INCLUDE
#include "note.h"
#else
#include "implement/note.h"
#endif


ClassMembers( PostIt, Note )

	const char *	color;

EndOfClassMembers;


#endif /* IMPL_POSTIT_H */
