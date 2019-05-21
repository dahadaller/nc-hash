#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([public-phash], [0.1], [])
dnl AC_CONFIG_SRCDIR([src/client.cpp])
dnl AC_CONFIG_HEADERS([config.h])
AC_LANG([C++])

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_CC

if test -z "$LD" ; then
	LD=$CXX
fi
AC_SUBST([LD])

dnl Checks for libraries.
dnl NOTE: this will add things to LIBS that don't always make sense.
AC_CHECK_LIB([gmp], [__gmpz_init])
AC_CHECK_LIB([gmpxx], [__gmp_istream_set_base])
AC_CHECK_LIB([fftw3], [fftw_cost])
AC_CHECK_LIB([GraphicsMagick], [DespeckleImage])
AC_CHECK_LIB([paillier], [paillier_enc],[],[NO_SYSTEM_LIBPAI=1])

dnl Checks for header files.
AC_CHECK_HEADERS([inttypes.h stddef.h stdint.h stdlib.h sys/socket.h unistd.h])
AC_CHECK_HEADERS([CImg.h],[],[NO_SYSTEM_CIMG=1])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([clock_gettime memset])

dnl handle local libpaillier
LOCAL_LPAI=""
LIBPAI=""
LPAICPPFLAGS=""
AC_ARG_WITH([local-libpaillier],
	[AS_HELP_STRING([--with-local-libpaillier=xxx],
					[path to local source tree of libpaillier])],
	[
		if test -n "$withval" ; then
			LOCAL_LPAI="$withval"
			LIBPAI="${LOCAL_LPAI}/libpaillier.a"
			LPAICPPFLAGS="-I${LOCAL_LPAI}"
		fi
		if ! test -f $LOCAL_LPAI/libpaillier.a ; then
			AC_MSG_WARN([Could not find libpaillier.a in $LOCAL_LPAI/
						 Is the local paillier library built?])
		fi
	]
)
AC_SUBST([LOCAL_LPAI])
AC_SUBST([LIBPAI])
AC_SUBST([LPAICPPFLAGS])

dnl handle local cimg
LOCAL_CIMG=""
CIMGCPPFLAGS=""
AC_ARG_WITH([local-cimg],
	[AS_HELP_STRING([--with-local-cimg=xxx],
					[path to local source tree of CImg])],
	[
		if test -n "$withval" ; then
			LOCAL_CIMG="$withval"
			CIMGCPPFLAGS="-I${LOCAL_CIMG}"
		fi
		if ! test -f $LOCAL_CIMG/CImg.h ; then
			AC_MSG_WARN([$LOCAL_CIMG/CImg.h not found.  Check path?])
		fi
	]
)
dnl AC_SUBST([LOCAL_CIMG])  (I don't think this is used externally...)
AC_SUBST([CIMGCPPFLAGS])

dnl extra build flags in case any libraries are in non-standard places.
AC_ARG_WITH([cflags],
	[AS_HELP_STRING([--with-cflags],
					[Specify additional flags to pass to compiler])],
	[
		if test -n "$withval"  &&  test "x$withval" != "xno"  &&  \
		    test "x${withval}" != "xyes"; then
			CFLAGS="$CFLAGS $withval"
		fi
	]
)
AC_ARG_WITH([cppflags],
	[AS_HELP_STRING([--with-cppflags],
					[Specify additional flags to pass to preprocessor])],
	[
		if test -n "$withval"  &&  test "x$withval" != "xno"  &&  \
		    test "x${withval}" != "xyes"; then
			CPPFLAGS="$CPPFLAGS $withval"
		fi
	]
)
AC_ARG_WITH([ldflags],
	[AS_HELP_STRING([--with-ldflags],
					[Specify additional flags to pass to linker])],
	[
		if test -n "$withval"  &&  test "x$withval" != "xno"  &&  \
		    test "x${withval}" != "xyes"; then
			LDFLAGS="$LDFLAGS $withval"
		fi
	]
)

dnl if no cimg or no libpaillier, force configure to fail
if test "$NO_SYSTEM_LIBPAI" = "1" && test -z $LOCAL_LPAI ; then
	AC_MSG_ERROR([libpaillier is required for this demo:
                 http://acsc.cs.utexas.edu/libpaillier/])
fi

if test "$NO_SYSTEM_CIMG" = "1" && test -z $LOCAL_CIMG ; then
	AC_MSG_ERROR([The CImg library is required for this demo:
                 https://github.com/dtschump/CImg])
fi

m4_define([conf_gen_files],[src/Makefile
							Makefile])
AC_CONFIG_FILES(conf_gen_files)
dnl save list for distclean target:
CONFIG_GEN_FILES="m4_normalize(conf_gen_files)"
AC_SUBST([CONFIG_GEN_FILES])

AC_OUTPUT
