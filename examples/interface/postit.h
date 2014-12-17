
#ifndef POSTIT_H
#define POSTIT_H

#include <ooc/ooc.h>

#include "note.h"

DeclareClass( PostIt, Note );

PostIt postit_new( void );

void postit_set_color( PostIt self, const char * color );

/* Virtual function definitions
 */

Virtuals( PostIt, Note )

EndOfVirtuals;

#endif  /* POSTIT_H */
