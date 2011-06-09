# Process this file with autoconf to produce a configure script.

# Initialize Autoconf and Automake
AC_INIT([Cities3D], [0.9], [], [Cities3D])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_HEADER([config.h])

# Check for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_INSTALL

# Check for and configure libtool
AC_DISABLE_STATIC
AC_LIBTOOL_DLOPEN
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

# Check for and configure gettext
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.14.5])

# Check for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_SIZE_T

# Check for library functions.
AC_FUNC_ERROR_AT_LINE
AC_HEADER_STDC
AC_FUNC_SELECT_ARGTYPES
AC_CHECK_FUNCS([memset pow select sqrt])

# Check for additional libraries.
	# FIXME Check for libGL and libGLU

# Check for additional header files.
	# FIXME Check for GL and GLU include files

# Check for boost-1.33.1 exactly
AM_PATH_BOOST([1_33_1], [boostFound=1])
if test "$boostFound" != 1; then
	AC_MSG_ERROR([
		<boost> must be installed on your system
		but the boost header files couldn't be found.

		Please check that the boost header and library files are
		installed in appropriate locations.

		You can specify the prefix where boost is installed by
		using --with-boost-prefix=PATH when running configure.
	])
fi

# Check for fmod-3.74.1
AM_PATH_FMOD([3.74.1], [fmodFound=1])
if test "$fmodFound" != 1; then
	# 3.7.5
    AM_PATH_FMOD([3.7.5], [fmodFound=1])
	if test "$fmodFound" != 1; then
		#4.2.4
		AM_PATH_FMOD([4.24.05], [fmodFound=1])
		AC_DEFINE([FMOD4], [], [Use fmod4 instead of fmod3])
	fi
fi
if test "$fmodFound" != 1; then
	AC_MSG_ERROR([
		<fmod> must be installed on your system but the
		fmod header and/or library files couldn't be found.

		Please check that the fmod header and library files are
		installed in appropriate locations.

		You can specify the prefix where fmod is installed by
		using --with-fmod-prefix=PATH when running configure.
	])
fi

# Check for glew-1.3.5 exactly
AM_PATH_GLEW([1.3.5], [glewFound=1])
if test "$glewFound" != 1; then
	AC_MSG_ERROR([
		<glew> must be installed on your system but the
		glew header and/or library files couldn't be found.

		Please check that the glew header and library files are
		installed in appropriate locations.

		You can specify the prefix where glew is installed by
		using --with-glew-prefix=PATH when running configure.
	])
fi

# Check for wxWidgets-2.8.0 or above
AM_OPTIONS_WXCONFIG
AM_PATH_WXCONFIG([2.8.0], [wxFound=1], [wxFound=0], [net,xml,adv,aui,core,gl,html])
if test "$wxFound" != 1; then
	AC_MSG_ERROR([
		<wxWidgets> must be installed on your system
		but wx-config script couldn't be found.

		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' command) is in LD_LIBRARY_PATH or
		equivalent variable and wxWidgets version is 2.3.4 or above.

		You can specify the prefix where wxWidgets is installed by
		using --with-wx-prefix=PATH when running configure.
	])
fi

# Check for CitiesOnline
AM_PATH_CITIESONLINE

# Check for crypto++
AM_PATH_CRYPTOPP([5.5.2], [cryptoppFound=1])

# Check for upnp lib (on win32 upnp is always enabled)
AC_LIB_UPNP

# Check for --enable-debug and --disable-debug
AC_ARG_ENABLE(debug, 
    [  --enable-debug          compile with debugging info], 
    [ useDebug="$enableval"]
)

# Check for --enable-universal and --disable-universal
AC_ARG_ENABLE(universal, 
    [  --disable-universal     do not build universal binaries on macosx], 
    [ buildUniversal="$enableval" ], 
    [ buildUniversal="yes"]
)

# Reset default compilation and linking flags
CPPFLAGS=""
CFLAGS=""
CXXFLAGS=""
LDFLAGS=""

# Create custom compilation flags
if test "$useDebug" = "yes"; then
	# Enable debugging symbols only in debug mode
	C3D_CXXFLAGS="-O0 -g -D_DEBUG"
else
	# Enable optimization and symbol stripping only in non-debug mode
	C3D_CXXFLAGS="-O3 -s -DNDEBUG"
fi
# Enable precompiled wxWidgets headers
C3D_CXXFLAGS="$C3D_CXXFLAGS -DWX_PRECOMP"
# Enable all standard and extra warnings
C3D_CXXFLAGS="$C3D_CXXFLAGS -Wall -Wextra"
# Enable warning when precompiled headers cannot be used
C3D_CXXFLAGS="$C3D_CXXFLAGS -Winvalid-pch"
# Disable warning when member initializers are reordered or variables are unused
C3D_CXXFLAGS="$C3D_CXXFLAGS -Wno-reorder -Wno-unused"
# Mimic default hidden symbol visibility of win32 compilers
# C3D_CXXFLAGS="$C3D_CXXFLAGS -fvisibility=hidden -fvisibility-inlines-hidden"

if test "$buildUniversal" = "yes"; then
	# Build universal binaries on MacOSX
	C3D_OSX_CXXFLAGS="-arch i386 -arch ppc -isysroot /Developer/SDKs/MacOSX10.4u.sdk"
	C3D_OSX_LDFLAGS="$C3D_OSX_CXXFLAGS"
else
	# Don't build universal binaries
	C3D_OSX_CXXFLAGS=""
	C3D_OSX_LDFLAGS=""
fi

# Create custom linking flags
C3D_LDFLAGS=""

# Export compilation and linking flags
AC_SUBST(CPPFLAGS)
AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(C3D_CXXFLAGS)
AC_SUBST(C3D_LDFLAGS)
AC_SUBST(C3D_OSX_CXXFLAGS)
AC_SUBST(C3D_OSX_LDFLAGS)

# Output Makefiles
AC_CONFIG_FILES([Makefile src/Makefile distribute/Makefile po/Makefile.in])
AC_OUTPUT
