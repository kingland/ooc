#ifndef FLAVOUR_H
#define FLAVOUR_H

#include <ooc/ooc.h>

DeclareMixinInterface( Flavour )

	void 			(* set 			)( Object, const char * flavour );
	const char * 	(* taste 		)( Object );

EndOfInterface;

#endif  /* FLAVOUR_H */
