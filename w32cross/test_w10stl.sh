#!/bin/sh -e

# This script attempts to build the STL.
# In theory, doing this is useless.
# However, it does actually have a purpose.
export W32CROSS_SDKID=w10

. common/cbase.sh

"$IVTW_CL" /LD /MD /Itests/stl16inc downloaded/stl16/stl/src/*.cpp /out:tests/bin/msvcp140.dll
