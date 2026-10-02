#!/bin/sh -e

W32CROSS_SDKID=w10

. common/cbase.sh

rm -rf "${IVTW_SDKPFX}stl"
mkdir -p "${IVTW_SDKPFX}stl"

cp -r "${IVTW_DLPFX}stl16/stl/src" "${IVTW_SDKPFX}stl/src"
# should be transpositional
cp -r "${IVTW_DLPFX}stl16/stl/inc" "${IVTW_SDKPFX}stl/include"
cp "${IVTW_DLPFX}stl16/LICENSE.txt" "${IVTW_SDKPFX}licenses/MicrosoftSTL_LICENSE.txt"
cp "${IVTW_DLPFX}stl16/NOTICE.txt" "${IVTW_SDKPFX}licenses/MicrosoftSTL_NOTICE.txt"
# csetjmp is a Problem because of various disagreements on how setjmp 'should' work.
# usually, though, when clangd forcibly includes a C++ header, the C header is what is desired.
# so this hack should cover '99%' of cases.
echo "#include <setjmp.h>" > "${IVTW_SDKPFX}stl/include/csetjmp"

cp "notppl/include/"* "${IVTW_SDKPFX}stl/include/"

for arch in x86 x64 arm64; do
	mkdir -p "${IVTW_SDKPFX}stl/lib/$arch"
	ivtw_import_defs vc14_redist_stl "$arch" "${IVTW_SDKPFX}stl/lib/$arch"
	# Build the C++ link library. Note we can't build debug libs, as we don't have the defs for those STLs.
	"${IVTW_LIB}" "/machine:$arch" \
	"${IVTW_SDKPFX}stl/lib/$arch/msvcp140.lib" \
	"${IVTW_SDKPFX}stl/lib/$arch/msvcp140_1.lib" \
	"${IVTW_SDKPFX}stl/lib/$arch/msvcp140_atomic_wait.lib" \
	"${IVTW_SDKPFX}stl/lib/$arch/msvcp140_codecvt_ids.lib" \
	"/out:${IVTW_SDKPFX}stl/lib/$arch/msvcprt.lib"
done
