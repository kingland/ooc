
#ifndef IMPL_SAMPLE_H
#define IMPL_SAMPLE_H

#ifndef SAMPLE_H
#error Do #include "sample.h" right before doing #include "implement/sample.h"
#endif


!!! TODO !!! Remove this group if the class has B_a_s_e parent!
#ifndef NO_RELATIVE_INCLUDE
#include "base.h"
#else
#include "implement/base.h"
#endif


ClassMembers( Sample, Base )

	int		data;

EndOfClassMembers;


#endif /* IMPL_SAMPLE_H */
