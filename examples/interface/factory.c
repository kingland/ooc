
#include "factory.h"

size_t
rand_index( int upper_limit )
{
	return rand() % upper_limit;
}

const char *
get_an_owner()
{
	static const char * owners[] = {	"Duffy", "Ted", "Suzy", "Paul", "Peter", "Alice", "Jack",
										"Lemon", "Who?", "Mary", "Anett", "Joe", "Ann", "Edward"
									};

	return owners[ rand_index( sizeof(owners)/sizeof(const char *) ) ];
}

const char *
get_a_date()
{
	static const char * dates[] = {	"2012.01.01", "2011.12.13", "2011.10.28", "2011.08.26", "2011.07.23", "2011.08.01.", "2011.12.06",
									"2011.12.31", "2012.01.02", "2011.02.10", "2011.10.17", "2011.06.10", "2011.09.29", "2011.10.04"
									};

	return dates[ rand_index( sizeof(dates)/sizeof(const char *) ) ];
}

const char *
get_a_text()
{
	static const char * texts[] =
	{
		"text1", 
		"text2", 
		"text3", 
		"text4", 
		"text5", 
		"text6", 
		"text7", 
		"text8"
	};

	return texts[ rand_index( sizeof(texts)/sizeof(const char *) ) ];

}

const char *
get_a_color()
{
	static const char * colors[] = { "yellow", "green", "pink", "white"	};

	return colors[ rand_index( sizeof(colors)/sizeof(const char *) ) ];
}


/* Some unserializable classes
 */

ClassMembers( GlassOfWine, Base )
EndOfClassMembers;

Virtuals( GlassOfWine, Base )
EndOfVirtuals;

AllocateClass( GlassOfWine, Base );

static	void	GlassOfWine_initialize( Class this ) {}
static	void	GlassOfWine_finalize( Class this ) {}
static	void	GlassOfWine_constructor( GlassOfWine self, const void * params ) {}
static	void	GlassOfWine_destructor( GlassOfWine self, GlassOfWineVtable vtab ) {}
static	int	GlassOfWine_copy( GlassOfWine self, const GlassOfWine from ) { return OOC_COPY_DEFAULT; }

ClassMembers( Mouse, Base )
EndOfClassMembers;

Virtuals( Mouse, Base )
EndOfVirtuals;

AllocateClass( Mouse, Base );

static	void	Mouse_initialize( Class this ) {}
static	void	Mouse_finalize( Class this ) {}
static	void	Mouse_constructor( Mouse self, const void * params ) {}
static	void	Mouse_destructor( Mouse self, MouseVtable vtab ) {}
static	int	Mouse_copy( Mouse self, const Mouse from ) { return OOC_COPY_DEFAULT; }

ClassMembers( Keyboard, Base )
EndOfClassMembers;

Virtuals( Keyboard, Base )
EndOfVirtuals;

AllocateClass( Keyboard, Base );

static	void	Keyboard_initialize( Class this ) {}
static	void	Keyboard_finalize( Class this ) {}
static	void	Keyboard_constructor( Keyboard self, const void * params ) {}
static	void	Keyboard_destructor( Keyboard self, KeyboardVtable vtab ) {}
static	int	Keyboard_copy( Keyboard self, const Keyboard from ) { return OOC_COPY_DEFAULT; }

