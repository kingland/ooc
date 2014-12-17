
SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h) $(wildcard implement/*.h)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

TARGET  = mixin

CC = gcc
LL = gcc

debug : make_targets
debug : CPPFLAGS =
debug : CFLAGS = -O0 -g3
debug : LDFLAGS = -L/usr/local/lib/ooc
debug : LDLIBS = -looc_d

release : make_targets
release : CPPFLAGS = -DNDEBUG
release : CFLAGS = -O2
release : LDFLAGS = -L/usr/local/lib/ooc
release : LDLIBS = -looc

# Building the targets

make_targets : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(LL) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $(TARGET)

$(OBJECTS) : $(SOURCES) $(HEADERS)

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

% : %.o
	$(LL) $(LDFLAGS) $< $(LDLIBS) -o $@

clean :
	rm -f *.o
	rm -f $(TARGET)

