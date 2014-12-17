
#ifndef SAMPLE_H
#define SAMPLE_H

#include <ooc/ooc.h>

DeclareClass( Sample, Base );

Sample sample_new( void );

void sample_methods( Sample self, int );

/* Virtual function definitions
 */

Virtuals( Sample, Base )

	void	(* sample_virtual)  ( Sample );

EndOfVirtuals;

#endif  /* SAMPLE_H */
