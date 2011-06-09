#!/bin/bash

##### System info
SYSTEM_TYPE=$(uname -s)
SYSTEM_ARCH=$(uname -m)

##### Modify the following to the appropriate ABSOLUTE paths #####
BOOST_BASE="$PWD/boost/1_33_1"
FMOD_BASE="$PWD/fmod/3.7.4-linux"
GLEW_BASE="$PWD/glew/1.3.5"
WX_BASE="$PWD/wxWidgets/2.8.10"
CITIES3D_BASE="$PWD/Cities3D"
CRYPTOPP_BASE="$PWD/crypto++/5.5.2"

# 64bit fmod4 for linux
if test $SYSTEM_ARCH = "x86_64"; then
    FMOD_BASE="$PWD/fmod/4.2.4-linux64"
fi

# MacOSX
if test $SYSTEM_TYPE = "Darwin"; then
    FMOD_BASE="$PWD/fmod/3.7.5-mac"
    export MACOSX_DEPLOYMENT_TARGET=10.4
fi

##### Modify the following to the appropriate ABSOLUTE path #####
BUILD_ROOT="$PWD"
INSTALL_ROOT="$PWD"

##### Uncomment and modify any of these only if necessary #####
#export CPP=""
#export CC=""
#export CXX=""
#export LD=""
#export CPPFLAGS=""
#export CFLAGS=""
#export CXXFLAGS=""
#export LDFLAGS=""
#MAKEFLAGS="-j 5"

##### Convenient function to create build trees using configure and make #####
function ConfigureAndMake() {
    local BASE=$1
    local OPTIONS=$2
    local BUILD=$3
    local INSTALL=$4

    echo "########## Building $BUILD ##########"
    mkdir -p $BUILD
    cd $BUILD
    [[ $DO_CONFIGURE ]] && $BASE/configure --prefix=$INSTALL $OPTIONS
    [[ $DO_MAKE ]] && make $MAKEFLAGS && make install
    echo ""
}

##### Build BOOST using boost::build (which uses boost::jam) #####
function buildBOOST() {
    local BJAM_BASE="$BOOST_BASE/tools/build/jam_src"

    if [[ !(-d $BJAM_BASE/bin.linuxx86 && -x $BJAM_BASE/bin.linuxx86/bjam) ]]; then
        echo "########## Building $BJAM_BASE ##########"
        cd $BJAM_BASE
        ./build.sh gcc
    fi

    declare -a local BOOST_COMMON=("-sTOOLS=gcc"
                       "--layout=system"
                       "--without-date_time"
                       "--without-filesystem"
                       "--without-graph"
                       "--without-iostreams"
                       "--without-program_options"
                       "--without-python"
                       "--without-regex"
                       "--without-serialization"
                       "--without-signals"
                       "--without-test"
                       "--without-thread"
                       "--without-wave")
    declare -a local BOOST_DEBUG=("-sBUILD=debug\ \<runtime-link\>static\ \<threading\>multiple"
                      "--builddir=$BUILD_ROOT/boost-build-debug"
                      "--prefix=$INSTALL_ROOT/boost-install-debug")
    declare -a local BOOST_RELEASE=("-sBUILD=release\ \<runtime-link\>static\ \<threading\>multiple"
                        "--builddir=$BUILD_ROOT/boost-build-release"
                        "--prefix=$INSTALL_ROOT/boost-install-release")
                      

    if [ $DO_DEBUG ]; then
        echo "########## Building $BUILD_ROOT/boost-build-debug ##########"
        cd $BOOST_BASE
        $BJAM_BASE/bin.*/bjam "${BOOST_COMMON[@]}" "${BOOST_DEBUG[@]}" install
        echo ""
    fi
    
    if [ $DO_RELEASE ]; then
        echo "########## Building $BUILD_ROOT/boost-build-release ##########"
        cd $BOOST_BASE
        $BJAM_BASE/bin.*/bjam "${BOOST_COMMON[@]}" "${BOOST_RELEASE[@]}" install
        echo ""
    fi
}

##### Build FMOD (by mere copying) #####
function buildFMOD() {
    local INCDIR=$BUILD_ROOT/fmod-install/include
    local LIBDIR=$BUILD_ROOT/fmod-install/lib
    
    echo "########## Building $FMOD_BASE ##########"
    mkdir -p $INCDIR
    cp -f $FMOD_BASE/api/inc/*.h $INCDIR
    if test -f $FMOD_BASE/api/inc/fmod.hpp; then
        cp -f $FMOD_BASE/api/inc/*.hpp $INCDIR
    fi
    mkdir -p $LIBDIR

    if test $SYSTEM_TYPE = "Darwin"; then
        # generate universal binary of the fmod library
        lipo \
            -create \
            -arch i386 ${FMOD_BASE}/api/lib/libfmodx86.a \
            -arch ppc  ${FMOD_BASE}/api/lib/libfmod.a \
            -output $LIBDIR/libfmod-3.7.5.a
    else
        FMOD_LIB_PATH="$FMOD_BASE/api/lib"
        if test -d $FMOD_LIB_PATH; then
            cp -f $FMOD_LIB_PATH/*.so $LIBDIR
        else
            cp -f $FMOD_BASE/api/*.so $LIBDIR
        fi
    fi

    echo ""
}

##### Build GLEW using copy and make #####
function buildGLEW() {
    local BUILD=$BUILD_ROOT/glew-build
    local INSTALL=$BUILD_ROOT/glew-install
    
    echo "########## Building $GLEW_BASE ##########"
    mkdir -p $BUILD
    mkdir -p $INSTALL
    [[ $DO_CONFIGURE ]] && cp -fr $GLEW_BASE/* $BUILD
    
    # compile universal binaries on darwin
    echo -en 'CFLAGS.EXTRA+=-arch i386 -arch ppc\n' \
        >> $BUILD/config/Makefile.darwin
    echo -en 'CFLAGS.EXTRA+=-isysroot /Developer/SDKs/MacOSX10.4u.sdk\n' \
        >> $BUILD/config/Makefile.darwin
    echo -en 'LDFLAGS.EXTRA+=-arch i386 -arch ppc\n' \
        >> $BUILD/config/Makefile.darwin
    echo -en 'LDFLAGS.EXTRA+=-isysroot /Developer/SDKs/MacOSX10.4u.sdk\n' \
        >> $BUILD/config/Makefile.darwin
    
    [[ $DO_MAKE ]] && cd $BUILD && make \
        && cp -fr $BUILD/bin $BUILD/lib $BUILD/include $INSTALL
    echo ""
}

##### Build crypto++ using copy and make #####
function buildCRYPTOPP() {
    local BUILD=$BUILD_ROOT/crypto++-build
    local INSTALL=$BUILD_ROOT/crypto++-install

    echo "########## Building $CRYPTO_BASE ##########"
    mkdir -p $BUILD
    mkdir -p $INSTALL/lib
    mkdir -p $INSTALL/include
    cp -fr $CRYPTOPP_BASE/* $BUILD

    [[ $DO_MAKE ]] && cd $BUILD && make && \
        cp -f $BUILD/*.a $INSTALL/lib/ && \
        cp -f $BUILD/*.h $INSTALL/include/
    echo ""
}

##### Build WX using configure and make #####
function buildWX() {
    # FIXME: Use --disable-PACKAGE for unrequired libraries
    if test $SYSTEM_TYPE = "Darwin"; then
        local WX_COMMON="--enable-optimise --enable-unicode --disable-compat26 \
                --with-opengl --without-subdirs \
                CXXFLAGS=-I/usr/local/include LDFLAGS=-L/usr/local/lib"
        local WX_DEBUG="--enable-universal_binary --enable-debug"
        local WX_RELEASE="--enable-universal_binary"
    else
        local WX_COMMON="--enable-unicode --enable-shared --enable-optimize \
                --enable-stl --disable-compat26 --with-opengl --with-gtk --without-subdirs"
        local WX_RELEASE=""
        local WX_DEBUG="--enable-debug"
    fi

    if [ $DO_DEBUG ]; then
        ConfigureAndMake "$WX_BASE" \
                 "$WX_COMMON $WX_DEBUG" \
                 "$BUILD_ROOT/wxWidgets-build-debug" \
                 "$INSTALL_ROOT/wxWidgets-install-debug"
    fi
    
    if [ $DO_RELEASE ]; then
        ConfigureAndMake "$WX_BASE" \
                 "$WX_COMMON $WX_RELEASE" \
                 "$BUILD_ROOT/wxWidgets-build-release" \
                 "$INSTALL_ROOT/wxWidgets-install-release"
    fi
}

##### Crazy function to generate Makefile.am files for Cities3D #####
function GenerateCities3DMakefiles() {
    echo "########## Preparing $CITIES3D_BASE ##########"

    if test $SYSTEM_TYPE = "Darwin"; then
        return;
    fi

    echo  ">>>>> Updating $CITIES3D_BASE/src/Makefile.am ..."
    cd  $CITIES3D_BASE/src

    cat Makefile.am.template \
    | sed -e "s/^.*{{DATABASECORE}}.*$/..\/vcprojtofilelist --bulkbuild --project DatabaseCore\/DatabaseCore.vcproj/e" \
    | sed -e "s/^.*{{MODELCORE}}.*$/..\/vcprojtofilelist --bulkbuild --project ModelCore\/ModelCore.vcproj/e" \
    | sed -e "s/^.*{{RULESCORE}}.*$/..\/vcprojtofilelist --bulkbuild --project RulesCore\/RulesCore.vcproj/e" \
    | sed -e "s/^.*{{UICORE}}.*$/..\/vcprojtofilelist --bulkbuild --project UICore\/UICore.vcproj/e" \
    | sed -e "s/^.*{{GLCORE}}.*$/..\/vcprojtofilelist --bulkbuild --project GLCore\/GLCore.vcproj/e" \
    | sed -e "s/^.*{{CORERULES}}.*$/..\/vcprojtofilelist --bulkbuild --project CoreRules\/CoreRules.vcproj/e" \
    | sed -e "s/^.*{{NETWORKRULES}}.*$/..\/vcprojtofilelist --bulkbuild --project NetworkRules\/NetworkRules.vcproj/e" \
    | sed -e "s/^.*{{STANDARDRULES}}.*$/..\/vcprojtofilelist --bulkbuild --project StandardRules\/StandardRules.vcproj/e" \
    | sed -e "s/^.*{{VOLCANORULES}}.*$/..\/vcprojtofilelist --bulkbuild --project VolcanoRules\/VolcanoRules.vcproj/e" \
    | sed -e "s/^.*{{SEAFARERSRULES}}.*$/..\/vcprojtofilelist --bulkbuild --project SeafarersRules\/SeafarersRules.vcproj/e" \
    | sed -e "s/^.*{{CITIESRULES}}.*$/..\/vcprojtofilelist --bulkbuild --project CitiesRules\/CitiesRules.vcproj/e" \
    | sed -e "s/^.*{{TEXEDITOR}}.*$/..\/vcprojtofilelist --bulkbuild --project TexEditor\/TexEditor.vcproj/e" \
    | sed -e "s/^.*{{JSON_SPIRIT}}.*$/..\/vcprojtofilelist --project json_spirit\/json_spirit.vcproj/e" \
    | sed -e "s/^.*{{CITIESONLINERULES}}.*$/..\/vcprojtofilelist --bulkbuild --project CitiesOnlineRules\/CitiesOnlineRules.vcproj/e" \
    | sed -e "s/^.*{{CITIESONLINE}}.*$/..\/vcprojtofilelist --bulkbuild --project CitiesOnline\/CitiesOnline.vcproj/e" \
    | sed -e "s/^.*{{CITIES3D}}.*$/..\/vcprojtofilelist --bulkbuild --project Cities3D\/cities3d.vcproj/e" \
    > Makefile.am
    
    echo  ">>>>> Updating $CITIES3D_BASE/distribute/Makefile.am ..."
    cd  $CITIES3D_BASE/distribute

    cat Makefile.am.template \
    | sed -e "s/^.*{{OPTIONS}}.*$/ls -1 options\/* | sed -e \"s\/^.*$\/\t\& {{BR}}\/\"/e" \
    | sed -e "s/^.*{{SKINS}}.*$/ls -1 skins\/**\/* | sed -e \"s\/^.*$\/\t\& {{BR}}\/\"/e" \
    | sed -e "s/^.*{{SOUNDS}}.*$/ls -1 sounds\/**\/* | sed -e \"s\/^.*$\/\t\& {{BR}}\/\"/e" \
    | sed -e "s/^.*{{LOCALE}}.*$/ls -1 locale\/**\/* | sed -e \"s\/^.*$\/\t\& {{BR}}\/\"/e" \
    | sed -e "s/^.*{{TEX}}.*$/ls -1 tex\/**\/* | sed -e \"s\/^.*$\/\t\& {{BR}}\/\" -e \"\\\$ s\/ {{BR}}\/\/\"/e" \
    | sed -e "s/{{BR}}/\\\\/" \
    > Makefile.am

    echo ">>>>> Updating $CITIES3D_BASE/po/POTFILES.in ..."
    cd $CITIES3D_BASE
    echo "# List of source files which contain translatable strings." > po/POTFILES.in
    ls -1 src/**/*.h src/**/*.cpp >> po/POTFILES.in
    
    if test ! -x $CITIES3D_BASE/configure; then
        echo ">>>>> Generating $CITIES3D_BASE/configure ..."
        cd $CITIES3D_BASE
        autopoint
        autoreconf --symlink --install
    fi
}

##### Build CITIES3D using configure and make #####
function buildCITIES3D() {
    local CITIES3D_COMMON=""
    local CITIES3D_DEBUG="--disable-universal --enable-debug"
    local CITIES3D_RELEASE=""

    local BOOST_PREFIX_DEBUG="$INSTALL_ROOT/boost-install-debug"
    local BOOST_PREFIX_RELEASE="$INSTALL_ROOT/boost-install-release"
    local FMOD_PREFIX_DEBUG="$INSTALL_ROOT/fmod-install"
    local FMOD_PREFIX_RELEASE="$INSTALL_ROOT/fmod-install"
    local GLEW_PREFIX_DEBUG="$INSTALL_ROOT/glew-install"
    local GLEW_PREFIX_RELEASE="$INSTALL_ROOT/glew-install"
    local WX_PREFIX_DEBUG="$INSTALL_ROOT/wxWidgets-install-debug"
    local WX_PREFIX_RELEASE="$INSTALL_ROOT/wxWidgets-install-release"
    local CRYPTOPP_PREFIX_DEBUG="$INSTALL_ROOT/crypto++-install"
    local CRYPTOPP_PREFIX_RELEASE="$INSTALL_ROOT/crypto++-install"
    
    [[ $DO_GENERATE ]] && GenerateCities3DMakefiles

    if test $SYSTEM_TYPE = "Darwin"; then
        cd "$INSTALL_ROOT/Cities3D/mac" && \
            xcodebuild -configuration Debug
        return;
    fi

    if [ $DO_DEBUG ]; then
        ConfigureAndMake "$CITIES3D_BASE" \
                 "$CITIES3D_COMMON $CITIES3D_DEBUG \
                  --with-boost-prefix=$BOOST_PREFIX_DEBUG \
                  --with-fmod-prefix=$FMOD_PREFIX_DEBUG \
                  --with-glew-prefix=$GLEW_PREFIX_DEBUG \
                  --with-wx-prefix=$WX_PREFIX_DEBUG \
                  --with-cryptopp-prefix=$CRYPTOPP_PREFIX_DEBUG" \
                 "$BUILD_ROOT/Cities3D-build-debug" \
                 "$INSTALL_ROOT/Cities3D-install-debug"     

        (cd "$INSTALL_ROOT/Cities3D-install-debug/share" \
            && ln -fs Cities3D CitiesOnline)
        (cd "$INSTALL_ROOT/Cities3D-install-debug/lib/Cities3D" \
            && ln -fs $FMOD_PREFIX_DEBUG/lib/* .)
        (cd "$INSTALL_ROOT/Cities3D-install-debug/lib/Cities3D" \
            && ln -fs $GLEW_PREFIX_DEBUG/lib/* .)
        (cd "$INSTALL_ROOT/Cities3D-install-debug/lib/Cities3D" \
            && ln -fs $WX_PREFIX_DEBUG/lib/* .)
        (cd "$INSTALL_ROOT/Cities3D-install-debug/lib/Cities3D" \
            && ln -fs $CRYPTOPP_PREFIX_DEBUG/lib/* .)
    fi

    if [ $DO_RELEASE ]; then
        ConfigureAndMake "$CITIES3D_BASE" \
                 "$CITIES3D_COMMON $CITIES3D_RELEASE \
                  --with-boost-prefix=$BOOST_PREFIX_RELEASE \
                  --with-fmod-prefix=$FMOD_PREFIX_RELEASE \
                  --with-glew-prefix=$GLEW_PREFIX_RELEASE \
                  --with-wx-prefix=$WX_PREFIX_RELEASE \
                  --with-cryptopp-prefix=$CRYPTOPP_PREFIX_RELEASE" \
                 "$BUILD_ROOT/Cities3D-build-release" \
                 "$INSTALL_ROOT/Cities3D-install-release"
                 
        (cd "$INSTALL_ROOT/Cities3D-install-release/share/" \
            && ln -fs Cities3D CitiesOnline)
        (cd "$INSTALL_ROOT/Cities3D-install-release/lib/Cities3D" \
            && ln -fs $FMOD_PREFIX_RELEASE/lib/* .)
        (cd "$INSTALL_ROOT/Cities3D-install-release/lib/Cities3D" \
            && ln -fs $GLEW_PREFIX_RELEASE/lib/* .)
        (cd "$INSTALL_ROOT/Cities3D-install-release/lib/Cities3D" \
            && ln -fs $WX_PREFIX_RELEASE/lib/* .)
        (cd "$INSTALL_ROOT/Cities3D-install-release/lib/Cities3D" \
            && ln -fs $CRYPTOPP_PREFIX_RELEASE/lib/* .)
    fi
}

##### MAIN #####

for arg in $@; do
    [[ $arg == "checkout" ]] && DO_CHECKOUT=1
    [[ $arg == "update" ]] && DO_UPDATE=1

    [[ $arg == "generate" ]] && DO_GENERATE=1
    [[ $arg == "configure" ]] && DO_CONFIGURE=1
    [[ $arg == "make" ]] && DO_MAKE=1

    [[ $arg == "debug" ]] && DO_DEBUG=1
    [[ $arg == "release" ]] && DO_RELEASE=1

    [[ $arg == "boost" || $arg == "all" ]] && DO_BOOST=1
    [[ $arg == "fmod" || $arg == "all" ]] && DO_FMOD=1
    [[ $arg == "glew" || $arg == "all" ]] && DO_GLEW=1
    [[ $arg == "wxWidgets" || $arg == "all" ]] && DO_WX=1
    [[ $arg == "Cities3D" || $arg == "all" ]] && DO_CITIES3D=1

    [[ $arg == "cryptopp" || $arg == "all" ]] && DO_CRYPTOPP=1
done

if [[ ! ($DO_GENERATE || $DO_CONFIGURE || $DO_MAKE) ]]; then
    DO_GENERATE=1
    DO_CONFIGURE=1
    DO_MAKE=1
fi

if [[ ! ($DO_DEBUG || $DO_RELEASE) ]]; then
    DO_DEBUG=1
    DO_RELEASE=1
fi

[[ $DO_BOOST ]] && buildBOOST
[[ $DO_FMOD ]] && buildFMOD
[[ $DO_GLEW ]] && buildGLEW
[[ $DO_WX ]] && buildWX
[[ $DO_CITIES3D ]] && buildCITIES3D

[[ $DO_CRYPTOPP ]] && buildCRYPTOPP

if [[ ! ($DO_BOOST || $DO_FMOD || $DO_GLEW || $DO_WX || $DO_CRYPTOPP \
    || $DO_CITIES3D) ]]; then
    echo "Usage: ./build.sh [TASKS...] [MODES...] TARGETS... (args in any order)"
    echo "   TASKS = generate, configure, make (one or more space seperated)"
    echo "   MODES = debug, release (one or more space seperated)"
    echo "   TARGETS = boost, fmod, glew, wxWidgets, cryptopp, Cities3D (one or more)"
    echo "   (if TASKS and/or MODES is missing, all tasks and/or modes are enabled)"
fi

