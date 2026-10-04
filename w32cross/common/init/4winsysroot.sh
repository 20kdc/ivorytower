#!/bin/sh -e

. common/cbase.sh

# -- winsysroot --

rm -rf "${IVTW_SDKPFX}fakewinsysroot"
mkdir -p "${IVTW_SDKPFX}fakewinsysroot/VC/Tools/MSVC"
mkdir -p "${IVTW_SDKPFX}fakewinsysroot/Windows Kits/10/Include/10.0.19041.0"
mkdir -p "${IVTW_SDKPFX}fakewinsysroot/Windows Kits/10/Lib/10.0.19041.0"
mkdir -p "${IVTW_SDKPFX}fakewinsysroot/VC/Tools/MSVC/atlmfc"

ln -s "../../../../../wsdk/include" "${IVTW_SDKPFX}fakewinsysroot/Windows Kits/10/Include/10.0.19041.0/um"
ln -s "../../../../../wsdk/lib" "${IVTW_SDKPFX}fakewinsysroot/Windows Kits/10/Lib/10.0.19041.0/um"

ln -s "../../../../ucrt/include" "${IVTW_SDKPFX}fakewinsysroot/VC/Tools/MSVC/include"
ln -s "../../../../ucrt/lib" "${IVTW_SDKPFX}fakewinsysroot/VC/Tools/MSVC/lib"

ln -s "../../../../../stl/include" "${IVTW_SDKPFX}fakewinsysroot/VC/Tools/MSVC/atlmfc/include"
ln -s "../../../../../stl/lib" "${IVTW_SDKPFX}fakewinsysroot/VC/Tools/MSVC/atlmfc/lib"
