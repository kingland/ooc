testable_library = $(top_srcdir)/libs/libooc.a

check_PROGRAMS = testcasetest ooctest exceptiontest interfacetest refcountedtest listtest vectortest signaltest

check_LIBRARIES = libfakes.a
libfakes_a_SOURCES = bar.c barson.c bardaughter.c foo.c fooexception.c

test_suites = suite.sh

LDADD = ./libfakes.a $(testable_library)

TESTS = $(check_PROGRAMS) $(test_suites)

nobase_noinst_HEADERS= *.h implement/*.h

EXTRA_DIST = $(test_suites) *.bat Makefile.Windows MakefileEclipse.linux *.mcp win32-check-all.bat linux-check-all.sh

