#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([bitfield], [1.0.1], [vitalie@ciubotaru.tokyo])
AC_CONFIG_SRCDIR([bitfield.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([libbitfield.pc])

# Checks for programs.
AC_PROG_CC
AC_PROG_LN_S

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
#AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-lrt':
#AC_CHECK_LIB([rt], [main])

# Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h endian.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE

# Checks for library functions.
AC_FUNC_MALLOC

AC_CHECK_FUNCS_ONCE(popcount popcountl ffs ffsl flsl, , )

# Check for __builtin_popcountl
AC_MSG_CHECKING([for __builtin_popcountl()])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[]], [[
__builtin_popcountl(1UL);
return 0;
]])], [
AC_MSG_RESULT([yes])
AC_DEFINE([HAVE_BUILTIN_POPCOUNTL], 1,
          [Define to 1 if compiler provides __builtin_popcountl().])
], [
AC_MSG_RESULT([no])
])

# Check for __builtin_ffs
AC_MSG_CHECKING([for __builtin_ffs()])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[]], [[
__builtin_ffs(1);
return 0;
]])], [
AC_MSG_RESULT([yes])
AC_DEFINE([HAVE_BUILTIN_FFS], 1,
          [Define to 1 if compiler provides __builtin_ffs().])
], [
AC_MSG_RESULT([no])
])

# Check for __builtin_ffsl
AC_MSG_CHECKING([for __builtin_ffsl()])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[]], [[
__builtin_ffsl(1UL);
return 0;
]])], [
AC_MSG_RESULT([yes])
AC_DEFINE([HAVE_BUILTIN_FFSL], 1,
          [Define to 1 if compiler provides __builtin_ffsl().])
], [
AC_MSG_RESULT([no])
])

# Check for __builtin_clzl
AC_MSG_CHECKING([for __builtin_clzl()])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[]], [[
__builtin_clzl(1UL);
return 0;
]])], [
AC_MSG_RESULT([yes])
AC_DEFINE([HAVE_BUILTIN_CLZL], 1,
          [Define to 1 if compiler provides __builtin_clzl().])
], [
AC_MSG_RESULT([no])
])

# Check for __builtin_ctzl
AC_MSG_CHECKING([for __builtin_ctzl()])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[]], [[
__builtin_ctzl(1UL);
return 0;
]])], [
AC_MSG_RESULT([yes])
AC_DEFINE([HAVE_BUILTIN_CTZL], 1,
          [Define to 1 if compiler provides __builtin_ctzl().])
], [
AC_MSG_RESULT([no])
])

AX_COMPILE_CHECK_SIZEOF(unsigned short)

AX_COMPILE_CHECK_SIZEOF(unsigned int)

AX_COMPILE_CHECK_SIZEOF(unsigned long)

AX_COMPILE_CHECK_SIZEOF(unsigned long long)

#AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([Makefile tests/Makefile man/Makefile examples/Makefile])
AC_OUTPUT
