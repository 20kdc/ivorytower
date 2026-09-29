#!/bin/sh -e
# Download SDK prerequisites.

cd "$(dirname "$(readlink -e "$0")")"

if [ "$W32CROSS_SDKID" = "" ]; then
	export W32CROSS_SDKID=w10
fi

. common/cbase.sh

sdk_download
