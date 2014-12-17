
#ifndef NOTE_H
#define NOTE_H

#include <ooc/ooc.h>

#include "serializable.h"

DeclareClass( Note, Base );

Note note_new( void );

/* Virtual function definitions
 */

Virtuals( Note, Base )

	Interface( Serializable );

EndOfVirtuals;

#endif  /* NOTE_H */
