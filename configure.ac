# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
# Configure script for OPeNDAP Sql data handler

AC_PREREQ(2.59)
AC_INIT([sql_module], [0.0.2], [ccancellieri@hotmail.com, dods-tech@unidata.ucar.edu])
AC_CONFIG_AUX_DIR([conf])

AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([SQLModule.h])
#AC_CONFIG_HEADER([config.h])
AM_CONFIG_HEADER(config.h)
AX_PREFIX_CONFIG_H(sql_config.h)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_CHECK_FUNCS([strchr])

dnl Checks for specific libraries
AC_CHECK_LIBDAP([3.10.2],
 [
  LIBS="$LIBS $DAP_LIBS"
  CPPFLAGS="$CPPFLAGS $DAP_CFLAGS"
 ],
 [ AC_MSG_ERROR([Cannot find libdap])
])

AC_CHECK_BES([3.8.3],
[
],
[ AC_MSG_ERROR([Could not find bes library and headers])
])

OPENDAP_DEBUG_OPTION

AM_PATH_CPPUNIT(1.12.0,
	[AM_CONDITIONAL([CPPUNIT], [true])],
	[AM_CONDITIONAL([CPPUNIT], [false])])
	
AC_CONFIG_FILES([Makefile
				bin/Makefile
				templates/Makefile])
AC_OUTPUT
