
#ifndef DESKTOP_H
#define DESKTOP_H

#include <ooc/ooc.h>

#include "serializable.h"

DeclareClass( Desktop, Base );

Desktop desktop_new( void );

/* Virtual function definitions
 */

Virtuals( Desktop, Base )

	Interface( Serializable );

EndOfVirtuals;

#endif  /* DESKTOP_H */
