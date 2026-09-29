#!/bin/sh -e

W32CROSS_SDKID=w10

. common/cbase.sh

"${IVTW_SDKPFX}bin/w32cross-treecasefix" "${IVTW_SDKPFX}"

# Specific 'fix me!' cases.
ln -s appmodel.h "${IVTW_SDKPFX}Windows Kits/10/Include/10.0.19041.0/um/AppModel.h"
