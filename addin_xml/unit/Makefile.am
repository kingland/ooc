## Makefile for for unit tests for Automake tools
## 
## Modify the first section, as needed.
##
#################################################

## Define the library to be tested in the testable library variable
##
testable_library = ## example: $(top_srcdir)/libs/libmycode.a

## Define each individual test cases
## (Program file names without extension)
##
check_PROGRAMS = ## example: myfirsttest mysecondtest

## Define each source files that are necessery for testing, but are not individual executables
## (Fake classes, additional classes, etc. Source filenames with extensions!)
#
check_LIBRARIES = libfakes.a
libfakes_a_SOURCES = ## example: bar.c foo.c

## Define the test suites (shell scripts) if there are
## (Leave empty if there is no test suite!)
##
test_suites = suite.sh

################################################
## Usuallay you do not have to change the rest!

LDADD = ./libfakes.a $(testable_library)

TESTS = $(check_PROGRAMS) $(test_suites)

nobase_pkginclude_HEADERS= *.h implement/*.h

EXTRA_DIST = $(test_suites)

