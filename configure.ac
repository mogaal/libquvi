#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])

m4_include([m4/version.m4])
AC_INIT([quvi], [_QUVI_VERSION], [legatvs@gmail.com])

# Interface:
QUVI_CURRENT=_QUVI_CURRENT
QUVI_REVISION=_QUVI_REVISION
QUVI_AGE=_QUVI_AGE
AC_SUBST(QUVI_CURRENT)
AC_SUBST(QUVI_REVISION)
AC_SUBST(QUVI_AGE)

AC_CONFIG_SRCDIR([lib/quvi.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([config.aux])
AC_CONFIG_MACRO_DIR([m4])

AC_CANONICAL_SYSTEM
AC_GNU_SOURCE

AC_DEFINE_UNQUOTED([CANONICAL_TARGET], "$target", [...])
AC_DEFINE_UNQUOTED([CANONICAL_HOST], "$host", [...])

AM_INIT_AUTOMAKE([-Wall -Werror foreign dist-bzip2])

LT_INIT()
LT_PREREQ([2.2])

# Host specific.
NO_UNDEFINED=
case "$host" in
    *-*-mingw32* | *-*-cygwin*)
        AC_DEFINE([HOST_W32], [1], [...])
        host_w32=yes
        if [ test x"$enable_shared" != "xno" ]; then
            NO_UNDEFINED="-no-undefined"
        fi
    ;;
esac
EXTRA_LIBQUVI_LDFLAGS="$NO_UNDEFINED"
AC_SUBST(EXTRA_LIBQUVI_LDFLAGS)

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LN_S
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P

AC_PATH_PROG(GENGETOPT, gengetopt, no)
AM_CONDITIONAL([HAVE_GENGETOPT], [test x"$GENGETOPT" != "xno"])
AC_SUBST(GENGETOPT)

AC_PATH_PROG(DOXYGEN, doxygen, no)
AM_CONDITIONAL([HAVE_DOXYGEN], [test x"$DOXYGEN" != "xno"])
AC_SUBST(DOXYGEN)

AC_PATH_PROG(PERL, perl, no)
AM_CONDITIONAL([HAVE_PERL], [test x"$PERL" != "xno"])
AC_SUBST(PERL)

have_pod_checker=no
AC_MSG_CHECKING([for Pod::Checker])
if [ $PERL -mPod::Checker >/dev/null 2>&1 ]; then
    AC_MSG_RESULT([yes])
    have_pod_checker=yes
fi
AM_CONDITIONAL([HAVE_PERL_POD_CHECKER], [test x"$have_pod_checker" = "xyes"])

AC_PATH_PROG(POD2MAN, pod2man, no)
AM_CONDITIONAL([HAVE_POD2MAN], [test x"$POD2MAN" != "xno"])
AC_SUBST(POD2MAN)

AC_PATH_PROG(POD2HTML, pod2html, no)
AM_CONDITIONAL([HAVE_POD2HTML], [test x"$POD2HTML" != "xno"])
AC_SUBST(POD2HTML)

build_date=
AC_PATH_PROG(DATE, date, no)
if [ test x"$DATE" != "xno" ]; then
    build_date=`"$DATE" +%Y%m%d`
fi
AC_DEFINE_UNQUOTED([BUILD_DATE], "$build_date", [...])

# Checks for libraries.
AM_ICONV
PKG_CHECK_MODULES(libcurl, libcurl >= 7.18.0)
PKG_CHECK_MODULES(libpcre, libpcre >= 7.8)

# Checks for header files.
AC_HEADER_STDC # `stdlib.h', `stdarg.h', `string.h', ...
AC_CHECK_HEADERS([memory.h stdint.h stdlib.h limits.h stddef.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
if [ test x"$host_w32" = "xno" ]; then
    AC_FUNC_MALLOC
    AC_FUNC_REALLOC
fi
AC_CHECK_FUNCS([memset strdup strstr strtol strerror strcspn])
AC_FUNC_STRTOD

have_asprintf=no
AC_CHECK_FUNCS([vasprintf], [
    AC_CHECK_FUNCS([asprintf], [have_asprintf=yes])
])
AM_CONDITIONAL([NO_ASPRINTF], [test x"$have_asprintf" != "xyes"])

have_strupr=no
AC_CHECK_FUNCS([strupr], [have_strupr=yes])
AM_CONDITIONAL([NO_STRUPR], [test x"$have_strupr" != "xyes"])

have_strtok_r=no
AC_CHECK_FUNCS([strtok_r], [have_strtok_r=yes])
AM_CONDITIONAL([NO_STRTOK_R], [test x"$have_strtok_r" != "xyes"])

if [ test x"$have_strtok_r" = "xno" ]; then
    AC_CHECK_FUNCS([strchr strpbrk strspn])
fi

# --enable-quvi
AC_ARG_ENABLE([quvi],
    [AS_HELP_STRING([--enable-quvi],
        [build quvi(1) program @<:@default=yes@:>@])],
    [],
    [enable_quvi=yes])
AM_CONDITIONAL([ENABLE_QUVI], [test x"$enable_quvi" != "xno"])

# --enable-smut
AC_ARG_ENABLE([smut],
    [AS_HELP_STRING([--enable-smut],
        [build with adult website support @<:@default=no@:>@])],
    [],
    [enable_smut=no])
AM_CONDITIONAL([ENABLE_SMUT], [test x"$enable_smut" != "xno"])

if [ test x"$enable_smut" != "xno" ]; then
    AC_DEFINE([ENABLE_SMUT], [1], [Define to enable smut])
fi

# --enable-broken
AC_ARG_ENABLE([broken],
    [AS_HELP_STRING([--enable-broken],
        [build with broken website support @<:@default=no@:>@])],
    [],
    [enable_broken=no])
AM_CONDITIONAL([ENABLE_BROKEN], [test x"$enable_broken" != "xno"])

if [ test x"$enable_broken" != "xno" ]; then
    AC_DEFINE([ENABLE_BROKEN], [1], [Define to enable broken support])
fi


# --with-man
AC_ARG_WITH([man],
    [AS_HELP_STRING([--with-man],
        [install manual pages @<:@default=yes@:>@])],
    [],
    [with_man=yes])
AM_CONDITIONAL([WITH_MAN], [test x"$with_man" != "xno"])

# GNU ld?
AM_CONDITIONAL([HAVE_GNU_LD], [test x"$with_gnu_ld" = "xyes"])

AC_CONFIG_FILES([
    Makefile
    include/Makefile
    include/quvi/Makefile
    include/quvi/quvi.h
    lib/Makefile
    src/Makefile
    examples/Makefile
    tests/Makefile
    tests/quviTest.pm
    libquvi.pc
    doxygen.conf
])

AC_OUTPUT

AC_MSG_NOTICE([Configured to build quvi/libquvi:

    quvi version:   ${VERSION}
    host setup:     ${host}
    install prefix: ${prefix}
    compiler:       ${CC}
    cflags:         ${CFLAGS}

Options:
    build libquvi:  shared=${enable_shared} static=${enable_static}
    build quvi:     ${enable_quvi}
    adult support:  ${enable_smut}
    broken support: ${enable_broken}
    install manual: ${with_man}

Other:
    gengetopt:      ${GENGETOPT}
    doxygen:        ${DOXYGEN}
    perl:           ${PERL}
    Pod::Checker:   ${have_pod_checker}
    pod2man:        ${POD2MAN}
    pod2html:       ${POD2HTML}
])


