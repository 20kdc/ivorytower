#!/bin/sh -e

WINCROSS_DIR="`readlink -e "$0"`"
export WINCROSS_DIR="`dirname $WINCROSS_DIR`"

cd "$WINCROSS_DIR"

# This script will need rearrangement for any other SDK.
# Maybe it would be better to have different scripts for different arrangements, idk.

# Get the ISO if we haven't got it already.
ISO_PATH="downloaded/19041.5609.250311-1926.vb_release_svc_im_WindowsSDK.iso"
if [ ! -e "$ISO_PATH" ]; then
	mkdir -p downloaded
	wget "https://go.microsoft.com/fwlink/?linkid=2312004" -O "$ISO_PATH"
fi

# Get the STL if we haven't got it already.
if [ ! -e "stl16" ]; then
	git clone --depth=1 --branch vs-2019-16.10 https://github.com/microsoft/STL/ stl16
fi

# 'Prepare' (nuke) target and build directories.
rm -rf target build
mkdir -p target build

mkdir -p build/isoextract
mkdir -p build/msiextract

7z x -obuild/isoextract "$ISO_PATH"

# needed for windows.h/etc.
msiextract "build/isoextract/Installers/Windows SDK for Windows Store Apps Headers-x86_en-us.msi" -C build/msiextract
msiextract "build/isoextract/Installers/Windows SDK for Windows Store Apps Libs-x86_en-us.msi" -C build/msiextract
# not strictly necessary, but also, d3dcompiler_47.dll (not for ARM though), fxc.exe, dxc.exe, etc...
# someone WILL want these.
msiextract "Windows SDK for Windows Store Apps Tools-x86_en-us.msi" -C build/msiextract

msiextract "build/isoextract/Installers/Windows SDK Desktop Headers arm-x86_en-us.msi" -C build/msiextract
msiextract "build/isoextract/Installers/Windows SDK Desktop Headers arm64-x86_en-us.msi" -C build/msiextract
msiextract "build/isoextract/Installers/Windows SDK Desktop Headers x64-x86_en-us.msi" -C build/msiextract
msiextract "build/isoextract/Installers/Windows SDK Desktop Headers x86-x86_en-us.msi" -C build/msiextract

msiextract "build/isoextract/Installers/Windows SDK Desktop Libs arm64-x86_en-us.msi" -C build/msiextract
msiextract "build/isoextract/Installers/Windows SDK Desktop Libs x64-x86_en-us.msi" -C build/msiextract
msiextract "build/isoextract/Installers/Windows SDK Desktop Libs x86-x86_en-us.msi" -C build/msiextract

msiextract "build/isoextract/Installers/Universal CRT Headers Libraries and Sources-x86_en-us.msi" -C build/msiextract
msiextract "build/isoextract/Installers/Universal CRT Redistributable-x86_en-us.msi" -C build/msiextract

# We're done extracting files, harvest.

mv -T "build/msiextract/Program Files/Windows Kits/10/Include/10.0.19041.0" "target/include"
mv -T "build/msiextract/Program Files/Windows Kits/10/Lib/10.0.19041.0" "target/lib"
mv -T "build/msiextract/Program Files/Windows Kits/10/Redist/10.0.19041.0" "target/redist"

rm -r build/msiextract

# build and run fixerupper
clang++ fixerupper.cpp -o fixerupper
./fixerupper

exit 1
