#!/bin/sh -e

# So this is one of those 'trying to have and eat cake' moments.
# The mingw-w64 headers should be left un-touched in case any meddling ruins things; they're considered a golden standard.
# But they do need some touchups because we're not running MinGW.
# That in mind, the authoritative files are kept as-is along with a set of patches.
# This script will, if the files do not exist, create them.
# Conversely, if the files do exist, it turns them back into patches.
# This runs whenever notvcrt gets compiled.
# If `build.sh` is run (which runs a clean build), the include_ext directory is wiped.

graft() {
	# echo "$1 $2"
	mkdir -p notvcrt/include_ext
	if [ ! -e "notvcrt/include_ext/$2" ]; then
		patch -o "notvcrt/include_ext/$2" -i "notvcrt/ext/$2.patch" "$1"
	else
		diff --label "mingw" -u "$1" --label "notvcrt" "notvcrt/include_ext/$2" > "notvcrt/ext/$2.patch" || true
	fi
}

# ORIGINAL INCLUDE_EXT
# BE SURE ALSO TO ADD TO w10/4vcruntime.sh
graft thirdparty/mingw-w64-headers/eh.h eh.h
graft thirdparty/mingw-w64-headers/excpt.h excpt.h
graft thirdparty/mingw-w64-headers/setjmp.h setjmp.h
graft thirdparty/mingw-w64-headers/setjmpex.h setjmpex.h
graft thirdparty/mingw-w64-headers/stdint.h stdint.h
