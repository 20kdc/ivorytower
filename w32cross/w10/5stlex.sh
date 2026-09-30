#!/bin/sh -e

W32CROSS_SDKID=w10

. common/cbase.sh

cp -r "${IVTW_DLPFX}stl16/stl/inc/"* "${SDK_MSVCPFX}/include/"
cp "${IVTW_DLPFX}stl16/LICENSE.txt" "${IVTW_SDKPFX}/licenses/MicrosoftSTL_LICENSE.txt"
cp "${IVTW_DLPFX}stl16/NOTICE.txt" "${IVTW_SDKPFX}/licenses/MicrosoftSTL_NOTICE.txt"

# Compile for all supported architectures.
# Note that we can only compile DLL versions.
# We do compile debug versions, but they're half-hearted.
for arch in x86 x64 arm64; do
	echo "compiling: $arch"
	mkdir -p "${SDK_MSVCPFX}/obj/$arch"
	mkdir -p "${SDK_MSVCPFX}/lib/$arch"
	# Pure imports are managed here.
	ivtw_import_defs vc14_redist "$arch" "${SDK_MSVCPFX}/lib/$arch"
	# oldnames.lib gets included by default, so we need to have it.
	ivtw_import_defs oldnames "$arch" "${SDK_MSVCPFX}/lib/$arch"
done
