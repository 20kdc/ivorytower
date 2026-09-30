#!/bin/sh -e

cd "$(dirname "$(readlink -e "$0")")"

if [ "$W32CROSS_SDKID" = "" ]; then
	export W32CROSS_SDKID=w10
fi

. common/cbase.sh

echo "-- w32cross build $W32CROSS_SDKID --"

sdk_download

rm -rf "sdk_$W32CROSS_SDKID"
# This ensures consistency when the setup is updated.
rm -rf "notvcrt/include_ext"
mkdir -p "$IVTW_SDKPFX"
common/compilerinit.sh

sdk_build

# This ensures w32cross-wizard *knows* the SDK is ready.
cat <<EOF > "$IVTW_SDKPFX/etc/ready"
W32Cross $W32CROSS_SDKID SDK setup $(git describe --all --long || true) READY
EOF
