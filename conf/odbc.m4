# -*- mode: autoconf -*-
# Configure macros for UnixODBC
#
# Based on bes.m4 by Patrice Dumas, et al.

# AC_CHECK_ODBC([MINIMUM-VERSION [, ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
# Test for UnixODBC and define ODBC_CFLAGS and ODBC_LIBS.
# Check that the version is above MINIMUM-VERSION 
# use when linking with a c++ aware linker, with a c linker you may also
# need -lstdc++

AC_DEFUN([AC_CHECK_ODBC],
[
  odbc_min_version=m4_if([$1], [], [3.1.0], [$1])
  odbc_no=

  AC_PATH_PROG([ODBC_CONFIG], [odbc_config], [no])

  AC_MSG_CHECKING([for UnixODBC version >= $odbc_min_version])
  if test "$ODBC_CONFIG" = 'no' ; then
    odbc_no=yes
  else
    odbc_config_major_version=`$ODBC_CONFIG --version | sed 's/^\([[0-9]]\)*\.\([[0-9]]*\)\.\([[0-9]]*\)$/\1/'`
    odbc_config_minor_version=`$ODBC_CONFIG --version | sed 's/^\([[0-9]]\)*\.\([[0-9]]*\)\.\([[0-9]]*\)$/\2/'`
    odbc_config_micro_version=`$ODBC_CONFIG --version | sed 's/^\([[0-9]]\)*\.\([[0-9]]*\)\.\([[0-9]]*\)$/\3/'`
    odbc_min_major_version=`echo $odbc_min_version | sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    odbc_min_minor_version=`echo $odbc_min_version | sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    odbc_min_micro_version=`echo $odbc_min_version | sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

    odbc_config_is_lt=""
    if test $odbc_config_major_version -lt $odbc_min_major_version ; then
      odbc_config_is_lt=yes
    else
      if test $odbc_config_major_version -eq $odbc_min_major_version ; then
        if test $odbc_config_minor_version -lt $odbc_min_minor_version ; then
          odbc_config_is_lt=yes
        else
          if test $odbc_config_minor_version -eq $odbc_min_minor_version ; then
            if test $odbc_config_micro_version -lt $odbc_min_micro_version ; then
              odbc_config_is_lt=yes
            fi
          fi
        fi
      fi
    fi
    if test x$odbc_config_is_lt = xyes ; then
      odbc_no=yes
    else
      ODBC_LIBS="`$ODBC_CONFIG --libs`"
      ODBC_CPPFLAGS="`$ODBC_CONFIG --cflags`"
    fi
  fi
  if test x$odbc_no = x ; then
    AC_MSG_RESULT([yes])
    m4_if([$2], [], [:], [$2])
  else
    AC_MSG_RESULT([no])
    if test "$ODBC_CONFIG" = 'no' ; then
      AC_MSG_NOTICE([The odbc_config script could not be found.])
    else
      if test x$odbc_config_is_lt = xyes ; then
        AC_MSG_NOTICE([the installed UnixODBC is too old.])
      fi
    fi
    ODBC_LIBS=
    ODBC_CPPFLAGS=
    m4_if([$3], [], [:], [$3])
  fi
  AC_SUBST([ODBC_LIBS])
  AC_SUBST([ODBC_CPPFLAGS])
])
