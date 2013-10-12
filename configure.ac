#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])

m4_include([m4/version.m4])

AC_INIT([libquvi], m4_esyscmd([./gen-ver.sh -c | tr -d '\n']),
        [http://quvi.sf.net/bugs/],[],[http://quvi.sf.net/])

AC_DEFINE_UNQUOTED([BUILD_OPTS], "$@",
  [Define to configure invocation command line options])

# Interface
LT_CUR=libquvi_lt_cur
LT_REV=libquvi_lt_rev
LT_AGE=libquvi_lt_age
AC_SUBST([LT_CUR])
AC_SUBST([LT_REV])
AC_SUBST([LT_AGE])

AC_CONFIG_SRCDIR([src/api/new.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([config.aux])
AC_CONFIG_MACRO_DIR([m4])

AC_CANONICAL_TARGET
AC_USE_SYSTEM_EXTENSIONS

AC_DEFINE_UNQUOTED([CANONICAL_TARGET], "$target",
  [Define to canonical target])
AC_DEFINE_UNQUOTED([CANONICAL_BUILD], "$build",
  [Define to canonical build])
AC_DEFINE_UNQUOTED([CANONICAL_HOST], "$host",
  [Define to canonical host])

# GNU Automake 1.13 spews a warning about AM_GNU_GETTEXT (0.18.1,
# 0.18.2) using the deprecated AM_PROG_MKDIR_P macro. Do not specify
# '-Werror` in the options to work around this.
AM_INIT_AUTOMAKE([1.11.1 -Wall dist-xz no-dist-gzip tar-ustar])
AM_SILENT_RULES([yes])

# GNU Automake 1.12 requires this macro. Earlier versions do not
# recognize this macro. Work around this.
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

AM_GNU_GETTEXT_VERSION([0.18.1])
AM_GNU_GETTEXT([external])
# GETTEXT_PACKAGE is used by glib.
AC_DEFINE([GETTEXT_PACKAGE], [PACKAGE], [Define to the gettext package name])
AC_SUBST([GETTEXT_PACKAGE], [PACKAGE])

LT_INIT([disable-static])
LT_PREREQ([2.2.6])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O

AC_DEFINE_UNQUOTED([CFLAGS], "$CFLAGS", [Define to compiler flags])
AC_DEFINE_UNQUOTED([CC], "$CC", [Define to compiler])

AC_PATH_PROG([DOXYGEN], [doxygen], [no])
AM_CONDITIONAL([HAVE_DOXYGEN], [test x"$DOXYGEN" != "xno"])
AC_SUBST([DOXYGEN])

AC_PATH_PROG([A2X], [a2x], [no])
AM_CONDITIONAL([HAVE_A2X], [test x"$A2X" != "xno"])
AC_SUBST([A2X])
AS_IF([test x"$A2X" = "xno" && test -d "$srcdir/.git"],
  AC_MSG_ERROR([a2x is required to create man pages when building from git])])

AC_PATH_PROG([DATE], [date], [no])
AS_IF([test x"$DATE" != "xno"], [build_time=`$DATE +"%F %T %z"`])
AC_DEFINE_UNQUOTED([BUILD_TIME], ["$build_time"], [We have build time])

BUILD_TIME="$build_time"
AC_SUBST([BUILD_TIME])

AC_PATH_PROG([VALGRIND], [valgrind], [no])
AM_CONDITIONAL([HAVE_VALGRIND], [test x"$VALGRIND" != "xno"])
AC_SUBST([VALGRIND])

# Checks for libraries.
PKG_CHECK_MODULES([liblua], [lua >= 5.1], [], [
  PKG_CHECK_MODULES([liblua], [lua51 >= 5.1], [], [
    PKG_CHECK_MODULES([liblua], [lua5.1 >= 5.1], [], [
      PKG_CHECK_MODULES(liblua, [lua-5.1 >= 5.1])])
  ])
])
PKG_CHECK_MODULES([libproxy], [libproxy-1.0 >= 0.3.1])
PKG_CHECK_MODULES([libcurl], [libcurl >= 7.21])
PKG_CHECK_MODULES([glib], [glib-2.0 >= 2.24])
AM_PATH_LIBGCRYPT

# Checks for scripts.
pkg_scripts=libquvi-scripts-0.9

AC_ARG_WITH([scriptsdir],
  [AS_HELP_STRING([--with-scriptsdir=DIR],
    [Set libquvi-scripts directory to DIR. This negates the
    libquvi-scripts check.  DIR is assumed to contain the files
    installed with libquvi-scripts to the
      $prefix/share/libquvi-scripts/ directory.])],
  [scriptsdir=$withval
   versionfile=$scriptsdir/version],
  [PKG_CHECK_MODULES([libquvi_scripts], ["$pkg_scripts" >= 0.9])
    scriptsdir=`$PKG_CONFIG --variable scriptsdir $pkg_scripts 2>/dev/null`
    versionfile=`$PKG_CONFIG --variable versionfile $pkg_scripts 2>/dev/null`
    scriptsmodver=`$PKG_CONFIG --modversion $pkg_scripts 2>/dev/null`])

AS_IF([test -z "$scriptsdir"], [
  AC_MSG_ERROR([scriptsdir: variable not found])])

AS_IF([ ! test -d "$scriptsdir"], [
  AC_MSG_ERROR([$scriptsdir: does not exist or is not a directory])])

AS_IF([test -z "$versionfile"], [
  AC_MSG_ERROR([versionfile: variable not found])])

AS_IF([ ! test -f "$versionfile"], [
  AC_MSG_WARN([$versionfile: does not exist or is not not a regular file])])

AC_DEFINE_UNQUOTED([SCRIPTSDIR], ["$scriptsdir"], [We have scriptsdir])
AC_DEFINE_UNQUOTED([VERSIONFILE], ["$versionfile"], [We have versionfile])

scriptsver=`grep <"$versionfile" "version ="|awk '{print $3}' 2>/dev/null`

# Checks for header files.
AC_CHECK_HEADERS([stdint.h string.h locale.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([strrchr memset setlocale])

# Version.
VN=`$srcdir/gen-ver.sh`
AC_DEFINE_UNQUOTED([VN], ["$VN"], [We have version number from gen-ver.sh])

VN_C=`$srcdir/gen-ver.sh -c`
AC_SUBST([VN_C])

# Version: Major/Minor -pair, used to find the libquvi scripts.
# e.g. $prefix/share/libquvi-scripts/$maj.$min/
VERSION_MM=`$srcdir/gen-ver.sh -c -m`
AC_DEFINE_UNQUOTED([VERSION_MM], "$VERSION_MM", [Version major/minor])

# --with-manual
AC_ARG_WITH([manual],
  [AS_HELP_STRING([--with-manual],
    [install manual pages @<:@default=yes@:>@])],
  [],
  [with_manual=yes])
AM_CONDITIONAL([WITH_MANUAL], [test x"$with_manual" != "xno"])

# 'quvi-object' is currently a globally accessible module. Lua 5.2
# discourages this practice by removing the luaL_register (and adds the
# luaL_setfuncs) call from the C API. We'll dismiss that (for now) with:
AC_DEFINE([LUA_COMPAT_MODULE], [1],
  [Define for backwards-compatibility with Lua 5.1])

AC_CONFIG_FILES([
  Makefile
  doc/man3/Makefile
  doc/man7/Makefile
  doc/dox/Makefile
  doc/dox/Doxyfile
  doc/Makefile
  examples/Makefile
  examples/lib/Makefile
  src/misc/Makefile
  src/gcrypt/Makefile
  src/curl/Makefile
  src/net/Makefile
  src/api/Makefile
  src/api/doxy/main.dp
  src/lua/Makefile
  src/quvi-0.9/quvi/Makefile
  src/quvi-0.9/Makefile
  src/Makefile
  tests/Makefile
  tests/lib/Makefile
  libquvi-0.9.pc
  po/Makefile.in])
AC_OUTPUT

AC_MSG_NOTICE([
  version         ${VERSION}
  prefix          ${prefix}
  compiler        ${CC}
  cflags          ${CFLAGS}
System types
  target          ${target}
  build           ${build}
  host            ${host}
Install options
  with
  - manual        ${with_manual}
libquvi-scripts
  .pc modversion  ${scriptsmodver}
  versionfile     ${versionfile}
    version       ${scriptsver}
  scriptsdir      ${scriptsdir}])

# vim: set ts=2 sw=2 tw=72 expandtab:
