#!/bin/bash

INSTALLDIR=$1
BUILDDIR=$2

SCRIPTDIR="$(dirname $0)"
CONTENTS=""
PLUGINSDIR=""
APPNAME=""

if [[ ! -d $INSTALLDIR && ! -d $BUILDDIR ]]; then
	echo "Usage: $0 <installdir> <builddir>"
	exit 1
fi


function CopyPlugins()
{
	echo -en "Copying Plugins..."
	local PLUGINS=$(find $BUILDDIR -name "lib*Rules.a" -print)
	PLUGINS="$PLUGINS libTexEditor.a"
	mkdir -p "$PLUGINSDIR"
	for plugin in $PLUGINS
	do
		cp "$plugin" "$PLUGINSDIR/$(basename $plugin \
			| sed -Ee 's/^lib(.*)\.a$/\1/').bundle"
		echo -en "."
	done
	echo
}


function CopyDependentLibs()
{
	echo -en "Copying dependent libraries..."
	otool -L "${CONTENTS}/MacOS/$APPNAME" \
		| sed -e 's/(.*)//' \
		| grep -v '@executable_path' \
		| awk \
			-v instdir="$INSTALLDIR" \
			-v libdir="$CONTENTS/SharedSupport/libs" \
			-v f=0 '\
			function copy(a) { system("cp "a" "libdir"/$(basename "a")");f=1} \
			/libwx/   {copy($0)} \
			/libfmod/ {copy($0)} \
			/libGLEW/ {copy(instdir"/../glew-install/lib/$(basename "$0")")} \
			{if(f){printf "+"} else {printf "."}}'
	echo
}


function CreateBundleHierarchy()
{
	mkdir -p "$CONTENTS/MacOS"
	mkdir -p "$CONTENTS/SharedSupport/libs"
	mkdir -p "$CONTENTS/Resources"
}


function CreatePackageInfoData()
{
	local LOWERAPPNAME=$(echo "$APPNAME" | awk '{{print tolower($0)}}')
	local FILEN="${CONTENTS}/Info.plist"

	# Create PkgInfo
	echo -en "APPL????" > "${CONTENTS}/PkgInfo"

	# Create Info.plist
	echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
		> $FILEN
	echo "<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \
		\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" \
		>> $FILEN
	echo "<plist version=\"1.0\">" \
		>> $FILEN
	echo " <dict>" \
		>> $FILEN
	echo "  <key>CFBundleExecutable</key>" \
		>> $FILEN
	echo "  <string>${APPNAME}</string>" \
		>> $FILEN
	echo "  <key>CFBundleIconFile</key>" \
		>> $FILEN
	echo "  <string>${LOWERAPPNAME}.icns</string>" \
		>> $FILEN
	echo "  <key>CFBundleName</key>" \
		>> $FILEN
	echo "  <string>${APPNAME}</string>" \
		>> $FILEN
	echo " </dict>" \
		>> $FILEN
	echo "</plist>" \
		>> $FILEN

	# Copy Icon
	cp "${SCRIPTDIR}/${LOWERAPPNAME}.icns" "${CONTENTS}/Resources/"
}


function CreateAppBundle()
{
	local WITH_PLUGINS=$1
	
	APPNAME=$2
	CONTENTS="${INSTALLDIR}/${APPNAME}.app/Contents"
	PLUGINSDIR="$CONTENTS/PlugIns/plugins"

	echo "Creating MacOSX Application Bundle for ${APPNAME}..."

	CreateBundleHierarchy
	CreatePackageInfoData
	
	cp "${BUILDDIR}/${APPNAME}" "${CONTENTS}/MacOS/"
	
	CopyDependentLibs
	[[ $WITH_PLUGINS = true ]] && CopyPlugins
	
	${SCRIPTDIR}/update_install_names.sh "$INSTALLDIR" "$APPNAME"
	echo "Done!"
}


# Main
# CreateBundle <with_plugins> <appname>
CreateAppBundle true  "Cities3D"
