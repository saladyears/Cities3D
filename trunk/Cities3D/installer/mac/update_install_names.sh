#!/bin/bash

INSTALLDIR=$1
APPNAME=$2

CONTENTS="${INSTALLDIR}/${APPNAME}.app/Contents"
PLUGINSDIR="${CONTENTS}/PlugIns/plugins"

if [[ ! -d $CONTENTS ]]; then
	echo "Usage: $0 <installdir> <appname>"
	echo "'$CONTENTS' not found"
	exit 1
fi

# Recursively update install names
function UpdateInstallNames()
{
	local OBJ=$1
	local NEWPATH="@executable_path/../SharedSupport/libs"
	
	if [[ ! -f $OBJ ]]; then
		return
	fi
	
	local DEPLIBS=$(otool -L $OBJ | sed -e 's/(.*)//' \
		| grep -v '@executable_path' \
		| awk '\
			/libwx/    {print $0} \
			/libfmod/  {print $0} \
			/libGLEW/  {print $0} \
		')
		
	for deplib in $DEPLIBS
	do
		if [[ $(basename $deplib) = $(basename $OBJ) ]]; then
			continue;
		fi
		install_name_tool -change \
			"$deplib" \
			"$NEWPATH/$(basename $deplib)" \
			"$OBJ"
		UpdateInstallNames "${CONTENTS}/SharedSupport/libs/$(basename $deplib)"
	done
	
}

# Update executable
echo "Patching install names of ${APPNAME}..."
UpdateInstallNames "${CONTENTS}/MacOS/$APPNAME"

# Update plugins
if [[ -d $PLUGINSDIR ]]; then
	PLUGINS=$(find $PLUGINSDIR -name "*.bundle" -type f -print)
	echo " +-> Patching plugins..."
	for plugin in $PLUGINS
	do
		echo "     +-> $(basename $plugin)"
		UpdateInstallNames "$plugin"
	done
fi
