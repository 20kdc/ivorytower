#!/bin/sh -e

W32CROSS_SDKID=w10

. common/cbase.sh

sdk_isoextract

rm -rf "${IVTW_SDKPFX}msiextract"

# MSI extraction and mangling are a single stage.
# Basically this is just one big block of stuff which is per-SDK 'characterized.

# needed for windows.h/etc.
msiextract "${IVTW_SDKPFX}build/isoextract/Installers/Windows SDK for Windows Store Apps Headers-x86_en-us.msi" -C "${IVTW_SDKPFX}build/msiextract"
msiextract "${IVTW_SDKPFX}build/isoextract/Installers/Windows SDK for Windows Store Apps Libs-x86_en-us.msi" -C "${IVTW_SDKPFX}build/msiextract"
# not strictly necessary, but also, d3dcompiler_47.dll (not for ARM though), fxc.exe, dxc.exe, etc...
# someone WILL want these.
msiextract "${IVTW_SDKPFX}build/isoextract/Installers/Windows SDK for Windows Store Apps Tools-x86_en-us.msi" -C "${IVTW_SDKPFX}build/msiextract"

msiextract "${IVTW_SDKPFX}build/isoextract/Installers/Windows SDK Desktop Headers arm-x86_en-us.msi" -C "${IVTW_SDKPFX}build/msiextract"
msiextract "${IVTW_SDKPFX}build/isoextract/Installers/Windows SDK Desktop Headers arm64-x86_en-us.msi" -C "${IVTW_SDKPFX}build/msiextract"
msiextract "${IVTW_SDKPFX}build/isoextract/Installers/Windows SDK Desktop Headers x64-x86_en-us.msi" -C "${IVTW_SDKPFX}build/msiextract"
msiextract "${IVTW_SDKPFX}build/isoextract/Installers/Windows SDK Desktop Headers x86-x86_en-us.msi" -C "${IVTW_SDKPFX}build/msiextract"

msiextract "${IVTW_SDKPFX}build/isoextract/Installers/Windows SDK Desktop Libs arm64-x86_en-us.msi" -C "${IVTW_SDKPFX}build/msiextract"
msiextract "${IVTW_SDKPFX}build/isoextract/Installers/Windows SDK Desktop Libs x64-x86_en-us.msi" -C "${IVTW_SDKPFX}build/msiextract"
msiextract "${IVTW_SDKPFX}build/isoextract/Installers/Windows SDK Desktop Libs x86-x86_en-us.msi" -C "${IVTW_SDKPFX}build/msiextract"

msiextract "${IVTW_SDKPFX}build/isoextract/Installers/Universal CRT Headers Libraries and Sources-x86_en-us.msi" -C "${IVTW_SDKPFX}build/msiextract"
msiextract "${IVTW_SDKPFX}build/isoextract/Installers/Universal CRT Redistributable-x86_en-us.msi" -C "${IVTW_SDKPFX}build/msiextract"

# 'liability reasons'
msiextract "${IVTW_SDKPFX}build/isoextract/Installers/Windows SDK EULA-x86_en-us.msi" -C "${IVTW_SDKPFX}build/msiextract"

# Rearrangement

# clear existing rearrangement directories
rm -rf "${IVTW_SDKPFX}wbin" \
"${IVTW_SDKPFX}licenses" \
"${IVTW_SDKPFX}ucrt" \
"${IVTW_SDKPFX}cppwinrt" \
"${IVTW_SDKPFX}wsdk" \
"${IVTW_SDKPFX}redist"

# note we DO NOT cover STL here

kitroot="${IVTW_SDKPFX}build/msiextract/Program Files/Windows Kits/10"
kitver="10.0.19041.0"

# licenses

mkdir -p "${IVTW_SDKPFX}licenses"

mv -T "$kitroot/bin/$kitver" "${IVTW_SDKPFX}wbin"
rmdir "$kitroot/bin"
mv -T "$kitroot/Licenses/$kitver" "${IVTW_SDKPFX}licenses"
rmdir "$kitroot/Licenses"

# ucrt

mkdir -p "${IVTW_SDKPFX}ucrt/src"
mv -T "$kitroot/Include/$kitver/ucrt" "${IVTW_SDKPFX}ucrt/include"
mv -T "$kitroot/Lib/$kitver/ucrt" "${IVTW_SDKPFX}ucrt/lib"
mv -T "$kitroot/Lib/$kitver/ucrt_enclave" "${IVTW_SDKPFX}ucrt/lib/ucrt_enclave"
mv -T "$kitroot/Source/$kitver/ucrt" "${IVTW_SDKPFX}ucrt/src/ucrt"
rmdir "$kitroot/Source/$kitver"
rmdir "$kitroot/Source"

# This file gets in the way of Clang and we need it out of the way.
# We can always put in our own sensible one later and make it include this _after_ protecting against its 'quirks'.
mv "${IVTW_SDKPFX}ucrt/include/stddef.h" "${IVTW_SDKPFX}ucrt/include/__ucrt_stddef.h"

# cppwinrt

mv -T "$kitroot/Include/$kitver/cppwinrt" "${IVTW_SDKPFX}cppwinrt"

# wsdk

mkdir -p "${IVTW_SDKPFX}wsdk"
mv -T "$kitroot/Include/$kitver/um" "${IVTW_SDKPFX}wsdk/include"
mv -n "$kitroot/Include/$kitver/shared/"* "${IVTW_SDKPFX}wsdk/include/"
rmdir "$kitroot/Include/$kitver/shared"
mv -n "$kitroot/Include/$kitver/winrt/"* "${IVTW_SDKPFX}wsdk/include/"
rmdir "$kitroot/Include/$kitver/winrt"
rmdir "$kitroot/Include/$kitver"
rmdir "$kitroot/Include"
mv -T "$kitroot/Lib/$kitver/um" "${IVTW_SDKPFX}wsdk/lib"
rmdir "$kitroot/Lib/$kitver"
rmdir "$kitroot/Lib"

# redist

mv "$kitroot/Redist/$kitver/"* "$kitroot/Redist/"
rmdir "$kitroot/Redist/$kitver"
mv -T "$kitroot/Redist" "${IVTW_SDKPFX}redist"

# clean up isoextract
rm -rf "${IVTW_SDKPFX}build/isoextract"

# -- WSDK case hacks --

# downloaded/stl16/stl/src/winapisupp.cpp workaround
mv -T "${IVTW_SDKPFX}wsdk/include/appmodel.h" "${IVTW_SDKPFX}wsdk/include/AppModel.h"
# gl is typically called GL, and gl/GL.h expects as much
# of course, gl/GL.h is referred to as GL/gl.h by gl/GLU.h
mv -T "${IVTW_SDKPFX}wsdk/include/gl" "${IVTW_SDKPFX}wsdk/include/GL"
