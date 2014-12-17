
#ifndef SOFTDRINK_H
#define SOFTDRINK_H

#include <ooc/ooc.h>

#include "tonic.h"

DeclareClass( SoftDrink, Tonic );

SoftDrink softdrink_new( void );

Virtuals( SoftDrink, Tonic )
EndOfVirtuals;

#endif  /* SOFTDRINK_H */
