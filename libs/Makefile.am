
SOURCE_FILES = ooc.c exception.c refcounted.c list.c vector.c signal.c testcase.c

AM_CFLAGS= -I..
AUTOMAKE_OPTIONS = nostdinc

pkglib_LIBRARIES = libooc.a libooc_d.a libooc_st.a libooc_st_d.a

libooc_a_CFLAGS = -O2 -g -DNDEBUG -pthread $(AM_CFLAGS)
libooc_a_SOURCES= $(SOURCE_FILES)

libooc_d_a_CFLAGS = -O0 -g3 -pthread $(AM_CFLAGS)
libooc_d_a_SOURCES= $(SOURCE_FILES)

libooc_st_a_CFLAGS = -O2 -g -DNDEBUG -DOOC_NO_THREADS $(AM_CFLAGS)
libooc_st_a_SOURCES= $(SOURCE_FILES)

libooc_st_d_a_CFLAGS = -O0 -g3 -DOOC_NO_THREADS $(AM_CFLAGS)
libooc_st_d_a_SOURCES= $(SOURCE_FILES)

nobase_pkginclude_HEADERS= *.h implement/*.h port/*.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ooc.pc

EXTRA_DIST = libooc.vcproj *.dox NMakefile.MPLAB mcc30_ext.c mcc18_ext.c
