#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])

m4_include([m4/version.m4])

AC_INIT(
    [quvi], [_QUVI_VERSION],
    [http://sourceforge.net/apps/trac/quvi],
    [],
    [http://quvi.sourceforge.net/])

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

AM_INIT_AUTOMAKE([-Wall -Werror foreign dist-bzip2 dist-xz])

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

AC_PATH_PROG(DATE, date, no)
if [ test x"$DATE" != "xno" ]; then
    build_date=`"$DATE" +%Y-%m-%d 2>/dev/null`
    if [ test x"$build_date" != "x" ]; then
        AC_DEFINE_UNQUOTED([BUILD_DATE], "$build_date", [We have 'date'])
    fi
fi

AC_PATH_PROG(GIT, git, no)
if [ test x"$GIT" != "xno" ]; then
    QUVI_BRANCH=_QUVI_BRANCH
    git_describe=`"$GIT" describe "$QUVI_BRANCH" 2>/dev/null`
    if [ test x"$git_describe" != "x" ]; then
        AC_DEFINE_UNQUOTED([GIT_DESCRIBE],
            "$git_describe", [We have 'git describe'])
    fi
fi

# Checks for libraries.
AM_ICONV
PKG_CHECK_MODULES(libcurl, libcurl >= 7.18.0)
PKG_CHECK_MODULES(liblua, lua >= 5.1, [], [
    PKG_CHECK_MODULES(liblua, lua51 >= 5.1, [], [
        PKG_CHECK_MODULES(liblua, lua5.1 >= 5.1, [], [
            PKG_CHECK_MODULES(liblua, lua-5.1 >= 5.1)
        ])
    ])
])
AM_SILENT_RULES([yes])

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
AC_CHECK_FUNCS(
[memset strdup strstr strtol strerror strcspn getcwd strchr strrchr strspn])
AC_FUNC_STRTOD

have_asprintf=no
AC_CHECK_FUNCS([vasprintf], [
    AC_CHECK_FUNCS([asprintf], [have_asprintf=yes])
])
AM_CONDITIONAL([NO_ASPRINTF], [test x"$have_asprintf" != "xyes"])

# --enable-quvi
AC_ARG_ENABLE([quvi],
    [AS_HELP_STRING([--enable-quvi],
        [build quvi command line tool@<:@default=yes@:>@])],
    [],
    [enable_quvi=yes])
AM_CONDITIONAL([ENABLE_QUVI], [test x"$enable_quvi" != "xno"])

# --enable-examples
AC_ARG_ENABLE([examples],
    [AS_HELP_STRING([--enable-examples],
        [build examples @<:@default=no@:>@])],
    [],
    [enable_examples=no])
AM_CONDITIONAL([ENABLE_EXAMPLES], [test x"$enable_examples" != "xno"])

# --enable-nsfw
AC_ARG_ENABLE([nsfw],
    [AS_HELP_STRING([--enable-nsfw],
        [build with adult website support @<:@default=no@:>@])],
    [],
    [enable_nsfw=no])
AM_CONDITIONAL([ENABLE_NSFW], [test x"$enable_nsfw" != "xno"])

if [ test x"$enable_nsfw" != "xno" ]; then
    AC_DEFINE([ENABLE_NSFW], [1], [Define to enable nsfw])
fi

# --enable-tests
AC_ARG_ENABLE([tests],
    [AS_HELP_STRING([--enable-tests],
        [enable tests @<:@default=no@:>@])],
    [],
    [enable_tests=no])
AM_CONDITIONAL([ENABLE_TESTS], [test x"$enable_tests" != "xno"])

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

# --with-doc
AC_ARG_WITH([doc],
    [AS_HELP_STRING([--with-doc],
        [install documentation @<:@default=yes@:>@])],
    [],
    [with_doc=yes])
AM_CONDITIONAL([WITH_DOC], [test x"$with_doc" != "xno"])

# GNU ld?
AM_CONDITIONAL([HAVE_GNU_LD], [test x"$with_gnu_ld" = "xyes"])

AC_CONFIG_FILES([
    Makefile
    include/Makefile
    include/quvi/Makefile
    include/quvi/quvi.h
    lib/Makefile
    man1/Makefile
    share/Makefile
    doc/Makefile
    src/Makefile
    examples/Makefile
    tests/Makefile
    tests/quviTest.pm
    scripts/Makefile
    libquvi.pc
    doxygen.conf
])

AC_OUTPUT

AC_MSG_NOTICE([

Configured:
    quvi version:   ${VERSION}  g:${git_describe}  b:${build_date}
    host setup:     ${host}
    compiler:       ${CC}
    cflags:         ${CFLAGS}
    prefix:         ${prefix}

Options:
    build libquvi:  shared=${enable_shared} static=${enable_static}
    build examples: ${enable_examples}
    build quvi:     ${enable_quvi}
    --
    enable broken:  ${enable_broken}
    enable tests:   ${enable_tests}
    enable nsfw:    ${enable_nsfw}
    --
    install manual: ${with_man}
    install docs:   ${with_doc}
])

#Other:
#    gengetopt:      ${GENGETOPT}
#    doxygen:        ${DOXYGEN}
#    perl:           ${PERL}
#    Pod::Checker:   ${have_pod_checker}
#    pod2man:        ${POD2MAN}
#    pod2html:       ${POD2HTML}

