AC_INIT([interflop-backend-checkfloatmax],[1.0],[interflop.project@gmail.com])
AM_SILENT_RULES([yes])
AC_CONFIG_AUX_DIR(autoconf)
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror foreign])
AC_CONFIG_MACRO_DIRS([m4])
AC_PROG_CC
AC_PROG_CXX
AM_PROG_AR
AC_CONFIG_HEADERS([config.h])
LT_INIT
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

AX_INTERFLOP_STDLIB()
AX_WARNINGS()
AX_LTO()

AC_CONFIG_FILES([Makefile])
AC_OUTPUT