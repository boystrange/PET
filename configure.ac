dnl Process this file with autoconf to produce a configure script.
AC_INIT
AC_CONFIG_SRCDIR([src/pet.hh])

PET_MAJOR_VERSION=0
PET_MINOR_VERSION=1
PET_MICRO_VERSION=1
PET_VERSION=$PET_MAJOR_VERSION.$PET_MINOR_VERSION.$PET_MICRO_VERSION
PET_VERSION_INFO=`expr $PET_MAJOR_VERSION + $PET_MINOR_VERSION`:$PET_MICRO_VERSION:$PET_MINOR_VERSION

MAJOR_VERSION=$PET_MAJOR_VERSION
VERSION=$PET_VERSION

AC_SUBST(MAJOR_VERSION)
AC_SUBST(PET_VERSION)
AC_SUBST(PET_VERSION_INFO)

AC_ARG_ENABLE(
        profile,
        [  --enable-profile[=ARG]  include profiling information [default=no]],
        profile=$enableval,
        profile=no
)

AC_ARG_ENABLE(
	gmetadom,
	[  --enable-gmetadom[=ARG]  enable the GMetaDOM tests [default=auto]],
	enable_gmetadom=$enableval,
	enable_gmetadom="auto"
)

AC_ARG_ENABLE(
	libxml2,
	[  --enable-libxml2[=ARG]   enable the libxml2 tests [default=auto]],
	enable_libxml2=$enableval,
	enable_libxml2="auto"
)

dnl Automake configuration
dnl AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(pet, $PET_VERSION)
dnl AC_DISABLE_SHARED
AM_PROG_LIBTOOL

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LN_S
AC_ISC_POSIX

AC_CHECK_HEADERS(hash_map)
AC_CHECK_HEADERS(ext/hash_map)

have_libxml2="no"
PKG_CHECK_MODULES(XML, libxml-2.0 >= 2.0.0,
  AC_DEFINE(HAVE_LIBXML,1,[Define to 1 is libxml2 is installed])
  have_libxml2="yes",
  AC_MSG_WARN([could not find libxml2]))
AC_SUBST(XML_CFLAGS)
AC_SUBST(XML_LIBS)
AM_CONDITIONAL([COND_LIBXML2], [test "$enable_libxml2" = "yes" -o \( "$enable_libxml2" = "auto" -a "$have_libxml2" = "yes" \)]) 
AM_CONDITIONAL([COND_LIBXML2_READER], [test "$enable_libxml2_reader" = "yes" -o \( "$enable_libxml2_reader" = "auto" -a "$have_libxml2" = "yes" \)]) 

have_gmetadom="no"
PKG_CHECK_MODULES(DOM, gdome2-cpp-smart >= 0.1.8,
  AC_DEFINE(HAVE_GMETADOM,1,[Define to 1 if gmetadom is installed])
  have_gmetadom="yes",
  AC_MSG_WARN([could not find GMetaDOM]))
AC_SUBST(DOM_CFLAGS)
AC_SUBST(DOM_LIBS)
AM_CONDITIONAL([COND_GMETADOM], [test "$enable_gmetadom" = "yes" -o \( "$enable_gmetadom" = "auto" -a "$have_gmetadom" = "yes" \)])

CFLAGS="$CFLAGS -W -Wall"
CXXFLAGS="$CXXFLAGS -W -Wall"

if test $profile = yes; then
	CFLAFS="$CFLAGS -O0 -pg"
	CXXFLAGS="$CXXFLAGS -O0 -pg"
	AC_DEFINE(ENABLE_PROFILE,1,[Define to 1 to enable profiling options])
fi

AC_CONFIG_FILES([
 Makefile
 src/Makefile
 tests/Makefile
 homepage/Makefile
 pet.pc
])
AC_OUTPUT

