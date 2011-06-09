dnl ---------------------------------------------------------------------------
dnl AM_PATH_BOOST([ACTUAL-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl ---------------------------------------------------------------------------

AC_DEFUN([AM_PATH_BOOST], [
	AC_ARG_WITH(boost-prefix,
		    [  --with-boost-prefix=PREFIX   Prefix where Boost is installed (optional)],
		    boost_config_prefix="$withval",
		    boost_config_prefix="")

	boost_version=$1
	AC_MSG_CHECKING(for boost - version == $boost_version)

	boost_includedir="$boost_config_prefix/include"
	boost_libdir="$boost_config_prefix/lib"
	if test -d "$boost_includedir"; then
		AC_MSG_RESULT(yes)
		ifelse([$2], , :, [$2])     
		BOOST_CXXFLAGS="-I$boost_includedir"
		BOOST_LIBS="-L$boost_libdir"
	else
		AC_MSG_RESULT(no)
		ifelse([$3], , :, [$3])
	fi

	AC_SUBST(BOOST_CXXFLAGS)
	AC_SUBST(BOOST_LIBS)
])


dnl ---------------------------------------------------------------------------
dnl AM_PATH_FMOD([ACTUAL-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl ---------------------------------------------------------------------------

AC_DEFUN([AM_PATH_FMOD], [
	AC_ARG_WITH(fmod-prefix,
		    [  --with-fmod-prefix=PREFIX   Prefix where Fmod is installed (optional)],
		    fmod_config_prefix="$withval",
		    fmod_config_prefix="")

	fmod_version=$1
	AC_MSG_CHECKING(for fmod - version == $fmod_version)

	fmod_includedir="$fmod_config_prefix/include"
	fmod_libdir="$fmod_config_prefix/lib"
	fmod_libname="fmod-$fmod_version"
	fmod_path="$fmod_libdir/lib$fmod_libname.so"

	if test $(uname -m) = "x86_64"; then
		fmod_libname="fmodex64-$fmod_version"
		fmod_path="$fmod_libdir/lib$fmod_libname.so"
	fi

	if test $(uname -s) = "Darwin"; then
		fmod_path="$fmod_libdir/lib$fmod_libname.a"
	fi

	if test -d "$fmod_includedir" -a -e $fmod_path; then
		AC_MSG_RESULT(yes)
		ifelse([$2], , :, [$2])     
		FMOD_CXXFLAGS="-I$fmod_includedir"
		FMOD_LIBS="-L$fmod_libdir -l$fmod_libname"
	else
		AC_MSG_RESULT(no)
		ifelse([$3], , :, [$3])
	fi

	AC_SUBST(FMOD_CXXFLAGS)
	AC_SUBST(FMOD_LIBS)
])


dnl ---------------------------------------------------------------------------
dnl AM_PATH_GLEW([ACTUAL-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl ---------------------------------------------------------------------------

AC_DEFUN([AM_PATH_GLEW], [
	AC_ARG_WITH(glew-prefix,
		    [  --with-glew-prefix=PREFIX   Prefix where Glew is installed (optional)],
		    glew_config_prefix="$withval",
		    glew_config_prefix="")

	glew_version=$1
	AC_MSG_CHECKING(for glew - version == $glew_version)

	glew_includedir="$glew_config_prefix/include"
	glew_libdir="$glew_config_prefix/lib"
	glew_path="$glew_libdir/libGLEW.so.$glew_version";

	if test $(uname -s) = "Darwin"; then
		glew_path="$glew_libdir/libGLEW.$glew_version.dylib";
	fi

	if test -d "$glew_includedir" -a -e "$glew_path"; then
		AC_MSG_RESULT(yes)
		ifelse([$2], , :, [$2])     
		GLEW_CXXFLAGS="-I$glew_includedir"
		GLEW_LIBS="-L$glew_libdir -lGLEW"
	else
		AC_MSG_RESULT(no)
		ifelse([$3], , :, [$3])
	fi

	AC_SUBST(GLEW_CXXFLAGS)
	AC_SUBST(GLEW_LIBS)
])

dnl ---------------------------------------------------------------------------
dnl AM_PATH_CITIESONLINE([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl ---------------------------------------------------------------------------

AC_DEFUN([AM_PATH_CITIESONLINE], [

    AC_MSG_CHECKING(for CitiesOnline sources)

    citiesonline_dir="$srcdir/src/CitiesOnline"
	citiesonline_rules_dir="$srcdir/src/CitiesOnlineRules"
	citiesonline_found="no"

    if test -d "$citiesonline_dir" -a -d "$citiesonline_rules_dir"; then
        AC_MSG_RESULT(yes)
		citiesonline_found="yes"
        ifelse([$1], , :, [$1])
    else
        AC_MSG_RESULT(no)
        ifelse([$2], , :, [$2])
    fi

	AM_CONDITIONAL(ENABLE_CITIESONLINE, test $citiesonline_found = "yes")
])


dnl ---------------------------------------------------------------------------
dnl AM_OPTIONS_WXCONFIG
dnl
dnl adds support for --wx-prefix, --wx-exec-prefix, --with-wxdir and
dnl --wx-config command line options
dnl
dnl NOTE : This was copied verbatim from wxWindows' wxwin.m4 aclocal macro.
dnl ---------------------------------------------------------------------------

AC_DEFUN([AM_OPTIONS_WXCONFIG],
[
    AC_ARG_WITH(wxdir,
                [  --with-wxdir=PATH       Use uninstalled version of wxWidgets in PATH],
                [ wx_config_name="$withval/wx-config"
                  wx_config_args="--inplace"])
    AC_ARG_WITH(wx-config,
                [  --with-wx-config=CONFIG wx-config script to use (optional)],
                wx_config_name="$withval" )
    AC_ARG_WITH(wx-prefix,
                [  --with-wx-prefix=PREFIX Prefix where wxWidgets is installed (optional)],
                wx_config_prefix="$withval", wx_config_prefix="")
    AC_ARG_WITH(wx-exec-prefix,
                [  --with-wx-exec-prefix=PREFIX
                          Exec prefix where wxWidgets is installed (optional)],
                wx_config_exec_prefix="$withval", wx_config_exec_prefix="")
])

dnl ---------------------------------------------------------------------------
dnl AM_PATH_WXCONFIG(VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND
dnl                  [, WX-LIBS [, ADDITIONAL-WX-CONFIG-FLAGS]]]])
dnl
dnl Test for wxWidgets, and define WX_C*FLAGS, WX_LIBS and WX_LIBS_STATIC
dnl (the latter is for static linking against wxWidgets). Set WX_CONFIG_NAME
dnl environment variable to override the default name of the wx-config script
dnl to use. Set WX_CONFIG_PATH to specify the full path to wx-config - in this
dnl case the macro won't even waste time on tests for its existence.
dnl
dnl Optional WX-LIBS argument contains comma- or space-separated list of
dnl wxWidgets libraries to link against (it may include contrib libraries). If
dnl it is not specified then WX_LIBS and WX_LIBS_STATIC will contain flags to
dnl link with all of the core wxWidgets libraries.
dnl
dnl Optional ADDITIONAL-WX-CONFIG-FLAGS argument is appended to wx-config
dnl invocation command in present. It can be used to fine-tune lookup of
dnl best wxWidgets build available.
dnl
dnl Example use:
dnl   AM_PATH_WXCONFIG([2.6.0], [wxWin=1], [wxWin=0], [html,core,net], [--unicode --debug])
dnl
dnl NOTE : This was copied verbatim from wxWindows' wxwin.m4 aclocal macro.
dnl	   The only change made was to add WX_GL_LIBS and WX_GL_LIBS_STATIC.
dnl        Having them seperate is useful to reduce unnecessary linking.
dnl ---------------------------------------------------------------------------

dnl Helper macro for checking if wx version is at least $1.$2.$3, set's
dnl wx_ver_ok=yes if it is:
dnl
AC_DEFUN([_WX_PRIVATE_CHECK_VERSION],
[
    wx_ver_ok=""
    if test "x$WX_VERSION" != x ; then
      if test $wx_config_major_version -gt $1; then
        wx_ver_ok=yes
      else
        if test $wx_config_major_version -eq $1; then
           if test $wx_config_minor_version -gt $2; then
              wx_ver_ok=yes
           else
              if test $wx_config_minor_version -eq $2; then
                 if test $wx_config_micro_version -ge $3; then
                    wx_ver_ok=yes
                 fi
              fi
           fi
        fi
      fi
    fi
])

dnl
dnl Get the cflags and libraries from the wx-config script
dnl
AC_DEFUN([AM_PATH_WXCONFIG],
[
  dnl do we have wx-config name: it can be wx-config or wxd-config or ...
  if test x${WX_CONFIG_NAME+set} != xset ; then
     WX_CONFIG_NAME=wx-config
  fi

  if test "x$wx_config_name" != x ; then
     WX_CONFIG_NAME="$wx_config_name"
  fi

  dnl deal with optional prefixes
  if test x$wx_config_exec_prefix != x ; then
     wx_config_args="$wx_config_args --exec-prefix=$wx_config_exec_prefix"
     WX_LOOKUP_PATH="$wx_config_exec_prefix/bin"
  fi
  if test x$wx_config_prefix != x ; then
     wx_config_args="$wx_config_args --prefix=$wx_config_prefix"
     WX_LOOKUP_PATH="$WX_LOOKUP_PATH:$wx_config_prefix/bin"
  fi
  if test "$cross_compiling" = "yes"; then
     wx_config_args="$wx_config_args --host=$host_alias"
  fi

  dnl don't search the PATH if WX_CONFIG_NAME is absolute filename
  if test -x "$WX_CONFIG_NAME" ; then
     AC_MSG_CHECKING(for wx-config)
     WX_CONFIG_PATH="$WX_CONFIG_NAME"
     AC_MSG_RESULT($WX_CONFIG_PATH)
  else
     AC_PATH_PROG(WX_CONFIG_PATH, $WX_CONFIG_NAME, no, "$WX_LOOKUP_PATH:$PATH")
  fi

  if test "$WX_CONFIG_PATH" != "no" ; then
    WX_VERSION=""

    min_wx_version=ifelse([$1], ,2.2.1,$1)
    if test -z "$5" ; then
      AC_MSG_CHECKING([for wxWidgets version >= $min_wx_version])
    else
      AC_MSG_CHECKING([for wxWidgets version >= $min_wx_version ($5)])
    fi

    WX_CONFIG_WITH_ARGS="$WX_CONFIG_PATH $wx_config_args $5 $4"

    WX_VERSION=`$WX_CONFIG_WITH_ARGS --version 2>/dev/null`
    wx_config_major_version=`echo $WX_VERSION | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    wx_config_minor_version=`echo $WX_VERSION | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    wx_config_micro_version=`echo $WX_VERSION | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

    wx_requested_major_version=`echo $min_wx_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    wx_requested_minor_version=`echo $min_wx_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    wx_requested_micro_version=`echo $min_wx_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

    _WX_PRIVATE_CHECK_VERSION([$wx_requested_major_version],
                              [$wx_requested_minor_version],
                              [$wx_requested_micro_version])

    if test -n "$wx_ver_ok"; then

      AC_MSG_RESULT(yes (version $WX_VERSION))
      WX_LIBS=`$WX_CONFIG_WITH_ARGS --libs`
      WX_GL_LIBS=`$WX_CONFIG_WITH_ARGS --gl-libs`

      dnl is this even still appropriate?  --static is a real option now
      dnl and WX_CONFIG_WITH_ARGS is likely to contain it if that is
      dnl what the user actually wants, making this redundant at best.
      dnl For now keep it in case anyone actually used it in the past.
      AC_MSG_CHECKING([for wxWidgets static library])
      WX_LIBS_STATIC=`$WX_CONFIG_WITH_ARGS --static --libs 2>/dev/null`
      WX_GL_LIBS_STATIC=`$WX_CONFIG_WITH_ARGS --static --gl-libs 2>/dev/null`
      if test "x$WX_LIBS_STATIC" = "x"; then
        AC_MSG_RESULT(no)
      else
        AC_MSG_RESULT(yes)
      fi

      dnl starting with version 2.2.6 wx-config has --cppflags argument
      wx_has_cppflags=""
      if test $wx_config_major_version -gt 2; then
        wx_has_cppflags=yes
      else
        if test $wx_config_major_version -eq 2; then
           if test $wx_config_minor_version -gt 2; then
              wx_has_cppflags=yes
           else
              if test $wx_config_minor_version -eq 2; then
                 if test $wx_config_micro_version -ge 6; then
                    wx_has_cppflags=yes
                 fi
              fi
           fi
        fi
      fi

      dnl starting with version 2.7.0 wx-config has --rescomp option
      wx_has_rescomp=""
      if test $wx_config_major_version -gt 2; then
        wx_has_rescomp=yes
      else
        if test $wx_config_major_version -eq 2; then
           if test $wx_config_minor_version -ge 7; then
              wx_has_rescomp=yes
           fi
        fi
      fi
      if test "x$wx_has_rescomp" = x ; then
         dnl cannot give any useful info for resource compiler
         WX_RESCOMP=
      else
         WX_RESCOMP=`$WX_CONFIG_WITH_ARGS --rescomp`
      fi

      if test "x$wx_has_cppflags" = x ; then
         dnl no choice but to define all flags like CFLAGS
         WX_CFLAGS=`$WX_CONFIG_WITH_ARGS --cflags`
         WX_CPPFLAGS=$WX_CFLAGS
         WX_CXXFLAGS=$WX_CFLAGS

         WX_CFLAGS_ONLY=$WX_CFLAGS
         WX_CXXFLAGS_ONLY=$WX_CFLAGS
      else
         dnl we have CPPFLAGS included in CFLAGS included in CXXFLAGS
         WX_CPPFLAGS=`$WX_CONFIG_WITH_ARGS --cppflags`
         WX_CXXFLAGS=`$WX_CONFIG_WITH_ARGS --cxxflags`
         WX_CFLAGS=`$WX_CONFIG_WITH_ARGS --cflags`

         WX_CFLAGS_ONLY=`echo $WX_CFLAGS | sed "s@^$WX_CPPFLAGS *@@"`
         WX_CXXFLAGS_ONLY=`echo $WX_CXXFLAGS | sed "s@^$WX_CFLAGS *@@"`
      fi

      ifelse([$2], , :, [$2])

    else

       if test "x$WX_VERSION" = x; then
          dnl no wx-config at all
          AC_MSG_RESULT(no)
       else
          AC_MSG_RESULT(no (version $WX_VERSION is not new enough))
       fi

       WX_CFLAGS=""
       WX_CPPFLAGS=""
       WX_CXXFLAGS=""
       WX_LIBS=""
       WX_LIBS_STATIC=""
       WX_GL_LIBS=""
       WX_GL_LIBS_STATIC=""
       WX_RESCOMP=""
       ifelse([$3], , :, [$3])

    fi
  else

    WX_CFLAGS=""
    WX_CPPFLAGS=""
    WX_CXXFLAGS=""
    WX_LIBS=""
    WX_LIBS_STATIC=""
	WX_GL_LIBS=""
	WX_GL_LIBS_STATIC=""
    WX_RESCOMP=""

    ifelse([$3], , :, [$3])

  fi

  AC_SUBST(WX_CPPFLAGS)
  AC_SUBST(WX_CFLAGS)
  AC_SUBST(WX_CXXFLAGS)
  AC_SUBST(WX_CFLAGS_ONLY)
  AC_SUBST(WX_CXXFLAGS_ONLY)
  AC_SUBST(WX_LIBS)
  AC_SUBST(WX_LIBS_STATIC)
  AC_SUBST(WX_GL_LIBS)
  AC_SUBST(WX_GL_LIBS_STATIC)
  AC_SUBST(WX_VERSION)
  AC_SUBST(WX_RESCOMP)
])

dnl ---------------------------------------------------------------------------
dnl AM_PATH_CRYPTOPP([ACTUAL-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl ---------------------------------------------------------------------------

AC_DEFUN([AM_PATH_CRYPTOPP], [
    AC_ARG_WITH(cryptopp-prefix,
            [  --with-cryptopp-prefix=PREFIX   Prefix where crypto++ is installed (optional)],
            cryptopp_config_prefix="$withval",
            cryptopp_config_prefix="")

    cryptopp_version=$1
    AC_MSG_CHECKING(for crypto++)

    cryptopp_includedir="$cryptopp_config_prefix/include"
    cryptopp_libdir="$cryptopp_config_prefix/lib"
    cryptopp_path="$cryptopp_libdir/libcryptopp.a";

    if test -d "$cryptopp_includedir" -a -e "$cryptopp_path"; then
        AC_MSG_RESULT(yes)
        ifelse([$2], , :, [$2])     
        CRYPTOPP_CXXFLAGS="-I$cryptopp_includedir"
        CRYPTOPP_LIBS="-L$cryptopp_libdir -lcryptopp"
    else
        AC_MSG_RESULT(no)
        ifelse([$3], , :, [$3])
    fi

    AC_SUBST(CRYPTOPP_CXXFLAGS)
    AC_SUBST(CRYPTOPP_LIBS)
])

dnl ---------------------------------------------------------------------------
dnl check for libupnp
dnl ---------------------------------------------------------------------------

AC_DEFUN([AC_LIB_UPNP], [
  AH_TEMPLATE([HAVE_LIBUPNP], [Define if libupnp is available])
  AC_ARG_WITH(libupnp, [  --with-libupnp=DIR      prefix for upnp library files and headers], [
    if test "$withval" = "no"; then
      ac_upnp_path=
      $2
    elif test "$withval" = "yes"; then
      ac_upnp_path=/usr
    else
      ac_upnp_path="$withval"
    fi
  ],[ac_upnp_path=/usr])
  if test "$ac_upnp_path" != ""; then
    saved_CPPFLAGS="$CPPFLAGS"
    CPPFLAGS="$CPPFLAGS -I$ac_upnp_path/include/upnp"
    AC_CHECK_HEADER([upnp.h], [
      saved_LDFLAGS="$LDFLAGS"
      LDFLAGS="$LDFLAGS -L$ac_upnp_path/lib"
      AC_CHECK_LIB(upnp, UpnpInit, [
        AC_SUBST(UPNP_CPPFLAGS, [-I$ac_upnp_path/include/upnp])
        AC_SUBST(UPNP_LDFLAGS, [-L$ac_upnp_path/lib])
        AC_SUBST(UPNP_LIBS, [-lupnp])
	AC_DEFINE([HAVE_LIBUPNP])
        $1
      ], [
	:
        $2
      ])
      LDFLAGS="$saved_LDFLAGS"
    ], [
      AC_MSG_RESULT([not found])
      $2
    ])
    CPPFLAGS="$saved_CPPFLAGS"
  fi
])

