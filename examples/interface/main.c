
#include <ooc/exception.h>

#include "desktop.h"

#include <stdlib.h> /* for srand() */
#include <time.h>	/* for time() */

int
main( int argc, char * argv[] )
{
	Desktop	desktop;

	ooc_init_class( Exception );

	srand ( time(NULL) ); /* initialize random seed */

	desktop = desktop_new();
	{
		ooc_manage_object( desktop );

		ooc_get_interface_must_have( desktop, Serializable )->serialize( (Object) desktop, 0 );

		ooc_delete( (Object) ooc_pass( desktop ) );
	}

	ooc_finalize_all();

	return 0;
}