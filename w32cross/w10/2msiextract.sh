#!/bin/sh -e

. common/cbase.sh

sdk_isoextract

rm -rf "${IVTW_SDKPFX}msiextract"
mkdir -p "${IVTW_SDKPFX}msiextract"

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

# So a problem here is SDK layout.
# In short, LLVM really, REALLY wants us to give it an SDK with the 'proper layout'.
# /winsysroot is set to Program Files, which means we get the space-containing "Windows Kits" directory involved.
# We don't want to encourage a (potentially brittle in the Unix world) space-filled layout.
# But we definitely need to tell LLVM we are using UCRT...
# We will have to live with what LLVM wants us to do here, as much as I hate it, but we'll provide alternate arrangements.
mv -T "${IVTW_SDKPFX}build/msiextract/Program Files/Windows Kits" "${IVTW_SDKPFX}Windows Kits"

# clean up nicely
rm -rf "${IVTW_SDKPFX}build/msiextract" "${IVTW_SDKPFX}build/isoextract"
