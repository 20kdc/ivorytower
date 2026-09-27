#!/bin/sh -e

. common/cbase.sh

# We need a VCRuntime, and the SDK won't give us a real one.
# Luckily, the VCRuntime is basically libgcc but for VC. It's not even the STL.
# We *can* just write our own.
mkdir -p target/VC/Tools/MSVC
cp -r notvcruntime/* target/VC/Tools/MSVC
