#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)

AC_INIT([Object Oriented C], [1.3a], [old-fashioned@users.sourceforge.net], [ooc], [http://ooc-coding.sourceforge.net])

AC_CONFIG_SRCDIR([libs/ooc.c])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stddef.h stdlib.h string.h])
# AC_INCLUDES_DEFAULT

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_C_VOLATILE

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove memset])

# Per-target compilation
AM_PROG_CC_C_O

AC_CONFIG_FILES([Makefile
                 libs/Makefile
                 libs/ooc.pc
                 docs/Makefile
                 tools/Makefile
                 unit/Makefile
                 unit_mcc18/Makefile])

AC_OUTPUT
