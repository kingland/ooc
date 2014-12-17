AM_CFLAGS = `pkg-config --cflags glib-2.0`
AM_CPPFLAGS = -DNDEBUG -DTEMPLATE_DIR='"$(pkgdatadir)/template"' 

ooc_LDADD = -lglib-2.0

bin_PROGRAMS=ooc
ooc_SOURCES=main.c

nobase_dist_pkgdata_DATA=template/*.c template/*.h template/implement/*.h template/unit/*

EXTRA_DIST = tools.vcproj glib-vc90-2_0.vsprops
