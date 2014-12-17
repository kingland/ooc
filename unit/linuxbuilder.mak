# Builder file for overall linux check
#
# This is used by linux-check-all.sh batch file only
#
###################################################

SOURCES = $(wildcard *.c)
EXCLUDE = mplabSIMc32Ext.c

EFFECTIVE_SOURCES = $(filter-out $(EXCLUDE), $(SOURCES))
TESTS = $(filter $(wildcard *test.c), $(EFFECTIVE_SOURCES))
FAKES = $(filter-out $(TESTS), $(EFFECTIVE_SOURCES))
FAKEOBJECTS = $(patsubst %.c, %.o, $(FAKES))
TARGETS = $(patsubst %.c, %, $(TESTS))

LDOBJECTS = $(FAKEOBJECTS)

CC = gcc
LL = gcc

# linux-check-all targets

debug : make_targets
debug : TESTABLE_TARGETS = ../libs/libooc_d.a
debug : CPPFLAGS =
debug : CFLAGS = -O0 -g3 -fopenmp
debug : LDFLAGS = -L../libs -fopenmp
debug : LDLIBS = -looc_d

release : make_targets
release : TESTABLE_TARGETS = ../libs/libooc.a
release : CPPFLAGS =
release : CFLAGS = -O2 -fopenmp
release : LDFLAGS = -L../libs -fopenmp
release : LDLIBS = -looc

debug_st : make_targets
debug_st : TESTABLE_TARGETS = ../libs/libooc_st_d.a
debug_st : CPPFLAGS =
debug_st : CFLAGS = -O0 -g3 -DOOC_NO_THREADS
debug_st : LDFLAGS = -L../libs
debug_st : LDLIBS = -looc_st_d

release_st : make_targets
release_st : TESTABLE_TARGETS = ../libs/libooc_st.a
release_st : CPPFLAGS =
release_st : CFLAGS = -O2 -DOOC_NO_THREADS
release_st : LDFLAGS = -L../libs
release_st : LDLIBS = -looc_st

# Building the targets

make_targets : $(TARGETS)

$(TARGETS) : $(FAKEOBJECTS) $(TESTABLE_TARGETS)

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

% : %.o
	$(LL) $< $(LDOBJECTS) $(LDFLAGS) $(LDLIBS) -o $@

clean :
	rm -f *.o
	rm -f $(TARGETS)

