
#ifndef BOOKLET_H
#define BOOKLET_H

#include <ooc/ooc.h>

#include "serializable.h"


DeclareClass( Booklet, Base );

Booklet booklet_new( void );

/* Virtual function definitions
 */

Virtuals( Booklet, Base )

	Interface( Serializable );

EndOfVirtuals;

#endif  /* BOOKLET_H */
