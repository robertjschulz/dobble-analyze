#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# AC_PREREQ(2.59)

AC_PREREQ([2.68])
AC_INIT(dobble-analyzer, 1.0, [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([src/CMatrixSolver.h])
AC_CONFIG_HEADERS([config.h])

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lntl':
AC_CHECK_LIB([ntl], [main])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday pow])

# AC_CONFIG_FILES(Makefile src/Makefile)
AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
