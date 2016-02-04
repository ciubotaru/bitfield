#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([bitfield], [0.3], [vitalie@ciubotaru.tk])
AC_CONFIG_SRCDIR([bitfield.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_LN_S

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
#AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-lrt':
#AC_CHECK_LIB([rt], [main])

# Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE

# Checks for library functions.
AC_FUNC_MALLOC

#AC_CONFIG_FILES([Makefile])
AC_OUTPUT([Makefile] [tests/Makefile] [man/Makefile])
