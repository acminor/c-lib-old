#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([CLIB_Array], [0.1], [austin.chase.m@gmail.com])
AC_CONFIG_AUX_DIR([build_aux])

AM_INIT_AUTOMAKE([subdir-objects 1.14])
AC_CONFIG_SRCDIR([src/test_array.c])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile])

# Checks for programs.
AC_PROG_CC
LT_PREREQ([2.4])
LT_INIT

# Checks for libraries.
# TODO check for Cmocka

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC

AC_OUTPUT
