
dnl Process this file with autoconf to produce a configure script.
AC_INIT([mlnx_sai], [3.0.0000])
AC_CONFIG_SRCDIR([])

dnl use local config dir for extras
AC_CONFIG_AUX_DIR(config)

dnl we use auto cheader
AM_CONFIG_HEADER(config.h)

dnl Defines the Language - we need gcc linker ...
AC_LANG_C

dnl Auto make
AM_INIT_AUTOMAKE

dnl Provides control over re-making of all auto files
AM_MAINTAINER_MODE

dnl Required for cases make defines a MAKE=make ??? Why
AC_PROG_MAKE_SET

dnl Checks for programs.
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_CC

dnl We will use libtool for making ...
AC_PROG_LIBTOOL

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([sys/types.h stdio.h stdlib.h string.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE

dnl Checks for library functions.
#AC_CHECK_FUNCS([strchr strstr strtol strtoull regcomp regexec])

dnl Define an input config option to control debug compile
AC_ARG_ENABLE(debug,
[  --enable-debug    Turn on debugging],
[case "${enableval}" in
	yes) debug=true ;;
	no)  debug=false ;;
	*) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
esac],[debug=false])
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)

dnl Define an input config option to control complib path
AC_ARG_WITH(sxcomplib,
[  --with-sxcomplib=<dir> define SwitchX compatibility library directory],
AC_MSG_NOTICE(Using SwitchX compatibility library from:$with_sxcomplib),
with_sxcomplib="none")
SX_COMPLIB_PATH=/usr/local
if test "x$with_sxcomplib" != xnone; then
  SX_COMPLIB_PATH=$with_sxcomplib
fi
AC_SUBST(SX_COMPLIB_PATH)

dnl Define an input config option to control sxd lib path
AC_ARG_WITH(sxdlibs,
[  --with-sxdlibs=<dir> define SwitchX Driver libraries directory],
AC_MSG_NOTICE(Using SwitchX Driver libraries from:$with_sxdlibs),
with_sxdlibs="none")
SXD_LIBS_PATH=/usr/local
if test "x$with_sxdlibs" != xnone; then
  SXD_LIBS_PATH=$with_sxdlibs
fi
AC_SUBST(SXD_LIBS_PATH)

dnl Define an input config option to control applibs path
AC_ARG_WITH(applibs,
[  --with-applibs=<dir> define SwitchX Application libraries directory],
AC_MSG_NOTICE(Using SwitchX Application libraries from:$with_sxapplibs),
with_applibs="none")
APP_LIB_PATH=/usr/local
if test "x$with_applibs" != xnone; then
  APP_LIB_PATH=$with_applibs
fi
AC_SUBST(APP_LIB_PATH)

CFLAGS_SAI_INTERFACE_COMMON="-Wall -Wswitch -Wunused -Werror -fPIC -fno-strict-aliasing -Wextra -Wunused -Wno-unused-parameter"
AC_SUBST(CFLAGS_SAI_INTERFACE_COMMON)

dnl Create the following Makefiles
AC_OUTPUT([Makefile src/Makefile])
