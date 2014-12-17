/*
 * Light-weight mixin example
 *
 *  Created on: Sep 3, 2011
 *      Author: old-fashioned
 */

#include <stdio.h>

#include <ooc/ooc.h>
#include <ooc/exception.h>
#include <ooc/vector.h>

#include "tonic.h"
#include "icecream.h"
#include "softdrink.h"

static
void
fill_array( Vector vector )
{
	/* "randomly" fills an array
	 */

	vector_push_back( vector, softdrink_new() );
	vector_push_back( vector, icecream_new( "bisque" ) );
	vector_push_back( vector, icecream_new( "banana" ) );
	vector_push_back( vector, softdrink_new() );
	vector_push_back( vector, softdrink_new() );
	vector_push_back( vector, icecream_new( "banana" ) );
	vector_push_back( vector, softdrink_new() );
	vector_push_back( vector, softdrink_new() );
	vector_push_back( vector, icecream_new( "banana" ) );
	vector_push_back( vector, icecream_new( "bisque" ) );
	vector_push_back( vector, icecream_new( NULL ) );
	vector_push_back( vector, softdrink_new() );
	vector_push_back( vector, icecream_new( "bisque" ) );
	vector_push_back( vector, softdrink_new() );
	vector_push_back( vector, icecream_new( "banana" ) );
}

static
void
taste_a_tonic( Tonic tonic )
{
	Flavour flavoured = ooc_get_interface( (Object) tonic, Flavour );

	if( flavoured )

		printf( "This %s tastes %s\n", tonic_name( tonic ), flavoured->taste( (Object) tonic ) );

	else

		printf( "The %s is not flavoured\n", tonic_name( tonic ) );
}

int
main( int argc, char * argv[] )
{
	Vector tonics;

	ooc_init_class( Exception );
	ooc_init_class( Vector );

	tonics = vector_new_type( 20, Tonic, OOC_MANAGE );
	{
		ooc_manage_object( tonics );

		fill_array( tonics );

		vector_foreach( tonics, (vector_item_executor) taste_a_tonic, NULL );

		ooc_delete( (Object) ooc_pass( tonics ) );
	}

	return 0;
}
