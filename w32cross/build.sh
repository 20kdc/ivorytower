#!/bin/sh -e

cd "$(dirname "$(readlink -e "$0")")"

if [ "$W32CROSS_SDKID" = "" ]; then
	export W32CROSS_SDKID=w10
fi

. common/cbase.sh

echo "-- w32cross build $W32CROSS_SDKID --"

rm -rf "sdk_$W32CROSS_SDKID" "notvcrt/include_ext"
mkdir -p "$IVTW_SDKPFX"
common/compilerinit.sh

sdk_build
