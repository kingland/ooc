#ifndef SAMPLE_H
#define SAMPLE_H

#include <ooc/ooc.h>

DeclareMixinInterface( Sample )

	// Put your interface methods here
	void 	(* my_method1)( Object, .. other params .. );

EndOfInterface;

#endif  /* SAMPLE_H */
