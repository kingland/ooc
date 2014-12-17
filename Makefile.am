SUBDIRS = libs docs tools unit unit_mcc18

EXTRA_DIST = ooc.sln config.h examples

## Deleting all hidden and unnecessary files from the examples directory

dist-hook:
	rm -rf `find $(distdir)/examples -name \.\* -or -name *.o -or -type f -perm -o+rx`
