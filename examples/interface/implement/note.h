
#ifndef IMPL_NOTE_H
#define IMPL_NOTE_H

#ifndef NOTE_H
#error Do #include "note.h" right before doing #include "implement/note.h"
#endif

ClassMembers( Note, Base )

	const char *		date;
	const char *		owner;
	const char *		text;

EndOfClassMembers;


#endif /* IMPL_NOTE_H */
