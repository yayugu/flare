#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)
AC_INIT(flare, 1.0.0)
AC_CONFIG_SRCDIR(src/flared/flared.cc)
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(src/config.h)
AC_CANONICAL_HOST

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

AC_LANG(C++)

# Checks for libraries.
AC_ARG_WITH(boost,
	[  --with-boost            boost libraries],
	[CXXFLAGS="${CXXFLAGS} -I${withval}/include" LDFLAGS="${LDFLAGS} -L${withval}/lib"])
AC_CHECK_LIB(boost_program_options, main, [], [AC_CHECK_LIB(boost_program_options-mt, main, [], [AC_MSG_ERROR(boost_program_options not found)])])
AC_CHECK_LIB(boost_regex, main, [], [AC_CHECK_LIB(boost_regex-mt, main, [], [AC_MSG_ERROR(boost_regex not found)])])

AC_CHECK_LIB(pthread, pthread_create, [], [echo "libpthread not found"; exit 1])

AC_ARG_WITH(tokyocabinet,
	[  --with-tokyocabinet     tokyocabinet libraries],
	[CXXFLAGS="${CXXFLAGS} -I${withval}/include -I${withval}/include/tc" LDFLAGS="${LDFLAGS} -L${withval}/lib"])
AC_CHECK_LIB(tokyocabinet, main, [], [echo "libtokyocabinet not found"; exit 1])

# Checks for header files.
AC_STDC_HEADERS
AC_CHECK_HEADER(pthread.h, [], [echo "pthread.h not found"; exit1])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNC(gethostbyname_r, [AC_DEFINE([HAVE_GETHOSTBYNAME_R], [], [have gethostbyname_r() or not])])

AC_MSG_CHECKING(for gnu specific strerror_r)
AC_TRY_COMPILE([
#include <string.h>
#include <stdlib.h>
], [char* p = (char*)malloc(16); char* q = strerror_r(0, p, sizeof(16));], ac_strerror_r=yes, ac_strerror_r=no);
AC_MSG_RESULT($ac_strerror_r)
if test $ac_strerror_r = yes; then
	AC_DEFINE([HAVE_GNU_STRERROR_R], [], [have gnu specific strerror_r()])
fi

# And others

#   debug options
AC_ARG_ENABLE(debug, [AC_HELP_STRING([--enable-debug], [enable debug compile [default=no]])], [], [enable_debug="no"])
if test "x$enable_debug" = "xno"
then
	DEBUG_DEFS=""
else
	DEBUG_DEFS="-DDEBUG"
fi

AC_ARG_ENABLE(memory-allocation-check, [AC_HELP_STRING([--enable-memory-allocation-check], [enable memory allocation check procs [default=no]])], [], [enable_memory_allocation_check="no"])
if test "x$enable_memory_allocation_check" = "xno"
then
	DEBUG_DEFS="${DEBUG_DEFS}"
else
	DEBUG_DEFS="${DEBUG_DEFS} -DMM_ALLOCATION_CHECK"
fi

#   pthread options
PTHREAD_DEFS="-pthread -D_REENTRANT -D_THREAD_SAFE"

FLARE_CXXFLAGS="$DEBUG_DEFS $PTHREAD_DEFS"
FLARE_LDFLAGS=""

AC_SUBST(FLARE_CXXFLAGS)
AC_SUBST(FLARE_LDFLAGS)

# Outputs
AC_CONFIG_FILES([Makefile \
    src/Makefile \
	src/lib/Makefile \
	src/flared/Makefile \
	src/flarei/Makefile])
AC_OUTPUT