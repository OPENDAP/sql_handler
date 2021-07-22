# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
# Configure script for OPeNDAP Sql data handler

AC_PREREQ(2.67)
AC_INIT([sql_module], [1.0.1], [support@opendap.org])
AC_CONFIG_MACRO_DIR([conf])
AC_CONFIG_AUX_DIR([conf])

AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([SQLModule.h])
AM_CONFIG_HEADER(config.h)
AX_PREFIX_CONFIG_H(sql_config.h)

# library visioning: Update these when the interface changes. Generally,
# assume that the interface tracks the major and minor release numbers.
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
AC_CHECK_HEADERS([stdlib.h string.h sql.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_CHECK_FUNCS([strchr])

dnl Checks for specific libraries
AC_CHECK_LIBDAP([3.20.0],
 [
  LIBS="$LIBS $DAP_LIBS"
  CPPFLAGS="$CPPFLAGS $DAP_CFLAGS"
 ],
 [ AC_MSG_ERROR([Cannot find libdap])
])

AC_CHECK_BES([3.20.0],
[
],
[ AC_MSG_ERROR([Could not find bes library and headers])
])

AC_ARG_ENABLE([sqlite], [AC_HELP_STRING([--enable-sqlite], [Build the native SQLite components])],
  [AM_CONDITIONAL([BUILD_SQLITE], [true])],
  [AM_CONDITIONAL([BUILD_SQLITE], [false])])

dnl libiodbc_ok="no"
dnl AC_ARG_WITH([iodbc],
dnl    AC_HELP_STRING([--with-iodbc=path], [Use the iodbc library/dirvers at this location.]),
dnl    [if test "$withval" = "yes"
dnl     then
dnl         AC_MSG_ERROR([You must supply a path when using --with-iodbc.])
dnl     else
dnl 	    ac_save_path=$PATH
dnl 	    export PATH=$withval/bin:$PATH

dnl 	    AC_PATH_PROG([iODBC_CONFIG], [iodbc-config], [no])

dnl         PATH=$ac_save_path

dnl 	    ODBC_LIBS="`$iODBC_CONFIG --libs`"
dnl 	    ODBC_CPPFLAGS="`$iODBC_CONFIG --cflags`"

dnl         AC_MSG_NOTICE([ODBC_CPPFLAGS is $ODBC_CPPFLAGS])
dnl         AC_MSG_NOTICE([ODBC_LIBS is $ODBC_LIBS])
        
dnl         AC_DEFINE([IODBC],[1],[define if iODBC dirvers/lib are present])
dnl         libiodbc_ok="yes"
dnl     fi],
dnl    [])

dnl if test "$libiodb_ok" = "no"
dnl then
dnl     AC_PATH_PROG([iODBC_CONFIG], [iodbc-config], [no])
dnl     ODBC_LIBS="`$iODBC_CONFIG --libs`"
dnl     ODBC_CPPFLAGS="`$iODBC_CONFIG --cflags`"
dnl fi

dnl AC_SUBST([ODBC_LIBS])
dnl AC_SUBST([ODBC_CPPFLAGS])

AC_CHECK_ODBC([2.2.14], [], [AC_MSG_ERROR([Could not find UnixODBC library and headers])])

AM_PATH_CPPUNIT(1.12.0,
	[AM_CONDITIONAL([CPPUNIT], [true])],
	[AM_CONDITIONAL([CPPUNIT], [false])])
	
AC_CONFIG_FILES([Makefile
		 bin/Makefile
		 templates/Makefile
		 sqlite/Makefile])
		 
AC_CONFIG_TESTDIR(tests, [.])

AC_CONFIG_FILES([tests/Makefile tests/atlocal tests/package.m4])

AC_OUTPUT

