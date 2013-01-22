# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
# Configure script for OPeNDAP Sql data handler

AC_PREREQ(2.59)
AC_INIT([sql_module], [0.0.3], [ccancellieri@hotmail.com, dods-tech@unidata.ucar.edu])
dnl Old. AC_CONFIG_AUX_DIR([conf])
AC_CONFIG_MACRO_DIR([conf])
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([SQLModule.h])
#AC_CONFIG_HEADER([config.h])
AM_CONFIG_HEADER(config.h)
AX_PREFIX_CONFIG_H(sql_config.h)

dnl library visioning: Update these when the interface changes. Generally,
dnl assume that the interface tracks the major and minor release numbers.
LIB_BES_SQL_CURRENT=1
LIB_BES_SQL_AGE=0
LIB_BES_SQL_REVISION=1
AC_SUBST(LIB_BES_SQL_CURRENT)
AC_SUBST(LIB_BES_SQL_AGE)
AC_SUBST(LIB_BES_SQL_REVISION)
LIBBES_SQL_VERSION="$LIB_BES_SQL_CURRENT:$LIB_BES_SQL_REVISION:$LIB_BES_SQL_AGE"
AC_SUBST(LIBBES_SQL_VERSION)

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

dnl OPENDAP_DEBUG_OPTION

dnl FIXME Will the code build against iODBC?
dnl

AC_PATH_PROG([iODBC_CONFIG], [iodbc-config], [no])
ODBC_LIBS="`$iODBC_CONFIG --libs`"
ODBC_CPPFLAGS="`$iODBC_CONFIG --cflags`"

AC_SUBST([ODBC_LIBS])
AC_SUBST([ODBC_CPPFLAGS])

dnl AC_CHECK_ODBC([2.2.14], [], [AC_MSG_ERROR([Could not find UnixODBC library and headers])])

AM_PATH_CPPUNIT(1.12.0,
	[AM_CONDITIONAL([CPPUNIT], [true])],
	[AM_CONDITIONAL([CPPUNIT], [false])])
	
AC_CONFIG_FILES([Makefile
		 bin/Makefile
		 templates/Makefile])
		 
AC_CONFIG_TESTDIR(tests, [.])

AC_CONFIG_FILES([tests/Makefile tests/atlocal tests/package.m4])
AC_CONFIG_FILES([tests/generate_metadata_baseline.sh], [chmod +x tests/generate_metadata_baseline.sh])
AC_CONFIG_FILES([tests/generate_data_baseline.sh], [chmod +x tests/generate_data_baseline.sh])

AC_OUTPUT

