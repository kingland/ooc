#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <ooc/ooc.h>

DeclareInterface( Serializable )

	void 	(* serialize)( Object, int indent );

EndOfInterface;

#endif  /* SERIALIZABLE_H */
