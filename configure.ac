#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])

m4_include([m4/version.m4])

AC_INIT([libquvi], [_LIBQUVI_VERSION],
  [http://sourceforge.net/apps/trac/quvi/], [],
  [http://quvi.sourceforge.net/])

# Interface:
LIBQUVI_CURRENT=_LIBQUVI_CURRENT
LIBQUVI_REVISION=_LIBQUVI_REVISION
LIBQUVI_AGE=_LIBQUVI_AGE
AC_SUBST([LIBQUVI_CURRENT])
AC_SUBST([LIBQUVI_REVISION])
AC_SUBST([LIBQUVI_AGE])

AC_CONFIG_SRCDIR([src/libquvi/quvi_api.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([config.aux])
AC_CONFIG_MACRO_DIR([m4])

AC_CANONICAL_SYSTEM
AC_GNU_SOURCE

AC_DEFINE_UNQUOTED([CANONICAL_TARGET], "$target", [...])
AC_DEFINE_UNQUOTED([CANONICAL_HOST], "$host", [...])

AM_INIT_AUTOMAKE([-Wall -Werror foreign dist-bzip2 dist-xz])
AM_SILENT_RULES([yes])

LT_INIT()
LT_PREREQ([2.2])

# Checks for programs.
AC_PROG_CC

AC_PATH_PROGS([ND], [naturaldocs NaturalDocs], [],
  [$ND_PATH$PATH_SEPARATOR$PATH])
AM_CONDITIONAL([HAVE_ND], [test -n "$ND"])

# Checks for libraries.
PKG_CHECK_MODULES([liblua], [lua >= 5.1], [], [
  PKG_CHECK_MODULES([liblua], [lua51 >= 5.1], [], [
    PKG_CHECK_MODULES([liblua], [lua5.1 >= 5.1], [], [
      PKG_CHECK_MODULES(liblua, [lua-5.1 >= 5.1])])
  ])
])
PKG_CHECK_MODULES([libcurl], [libcurl >= 7.18.2])
AM_ICONV

# Checks for scripts.
PKG_CHECK_MODULES([libquvi_scripts], [libquvi-scripts >= 0.4.0])

scriptsdir=`$PKG_CONFIG --variable scriptsdir libquvi-scripts 2>/dev/null`
versionfile=`$PKG_CONFIG --variable versionfile libquvi-scripts 2>/dev/null`
scriptsmodver=`$PKG_CONFIG --modversion libquvi-scripts 2>/dev/null`

AS_IF([test -z "$scriptsdir"], [
  AC_MSG_ERROR([libquvi-scripts.pc: scriptsdir: variable not found])])

AS_IF([test -z "$versionfile"], [
  AC_MSG_ERROR([libquvi-scripts.pc: versionfile: variable not found])])

AC_DEFINE_UNQUOTED([SCRIPTSDIR], ["$scriptsdir"], [We have scriptsdir])
AC_DEFINE_UNQUOTED([VERSIONFILE], ["$versionfile"], [We have versionfile])

AC_ARG_WITH([soup],
  AS_HELP_STRING([--with-soup], [Build (examples) with libsoup]))

AS_IF([test "x$with_soup" = "xyes"], [
  # Either libsoup-gnome or libsoup.
  PKG_CHECK_MODULES([libsoup_gnome], [libsoup-gnome-2.4 >= 2.26], [
    AC_DEFINE([HAVE_LIBSOUP_GNOME], [1], [Have libsoup-gnome])
    libsoup_CFLAGS="$libsoup_gnome_CFLAGS"
    libsoup_LIBS="$libsoup_gnome_LIBS"], [
      PKG_CHECK_MODULES([libsoup], [libsoup-2.4 >= 2.26])])
  AC_SUBST([libsoup_CFLAGS])
  AC_SUBST([libsoup_LIBS])
])

AM_CONDITIONAL([WITH_SOUP], [test x"$with_soup" = "xyes"])

ND_FORMAT=HTML
AC_ARG_WITH([nd-output],
  [AS_HELP_STRING([--with-nd-output=FORMAT],
    [NaturalDocs output format (HTML or FramedHTML)])], [
    AS_IF([test -z "$ND"], [
      AC_MSG_ERROR([NaturalDocs not found. No use in setting output format])])
    case $withval in
      HTML|FramedHTML)
        ND_FORMAT=$withval ;;
      *)
        AC_MSG_ERROR([NaturalDocs does not support $withval output format]) ;;
    esac
  ])
AC_SUBST([ND_FORMAT])

# Checks for header files.
AC_CHECK_HEADERS(
  [memory.h stdarg.h stddef.h stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_TYPES([ptrdiff_t])
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([getcwd memset strdup strerror strrchr strstr])
AC_FUNC_REALLOC
AC_FUNC_MALLOC

have_asprintf=no
AC_CHECK_FUNCS([vasprintf], [
  AC_CHECK_FUNCS([asprintf], [have_asprintf=yes])])
AM_CONDITIONAL([NO_ASPRINTF], [test x"$have_asprintf" != "xyes"])

# Version.
VN=`$srcdir/gen-ver.sh $srcdir`
AC_DEFINE_UNQUOTED([VN], ["$VN"], [We have version number from gen-ver.sh])

# --with-examples
AC_ARG_WITH([examples],
  [AS_HELP_STRING([--with-examples],
    [build examples @<:@default=no@:>@])],
  [],
  [with_examples=no])
AM_CONDITIONAL([WITH_EXAMPLES], [test x"$with_examples" != "xno"])

# --with-manual
AC_ARG_WITH([manual],
  [AS_HELP_STRING([--with-manual],
    [install manual pages @<:@default=yes@:>@])],
  [],
  [with_manual=yes])
AM_CONDITIONAL([WITH_MANUAL], [test x"$with_manual" != "xno"])

AC_CONFIG_FILES([
  Makefile
  doc/Makefile
  doc/man3/Makefile
  examples/Makefile
  src/Makefile
  src/libquvi/Makefile
  src/libquvi/quvi/Makefile
  libquvi.pc])
AC_OUTPUT

AC_MSG_NOTICE([
  version:        ${VERSION} (${VN})
  host setup:     ${host}
  prefix:         ${prefix}
  cflags:         ${CFLAGS}
  compiler:       ${CC}
libquvi-scripts.pc:
  modversion:     ${scriptsmodver}
  scriptsdir:     ${scriptsdir}
  versionfile:    ${versionfile}
Options:
  build libquvi:  shared=${enable_shared} static=${enable_static}
  build examples: ${with_examples}
  install manual: ${with_manual}])
