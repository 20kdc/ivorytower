#!/bin/sh -e

W32CROSS_SDKID=w10

. common/cbase.sh

"${IVTW_SDKPFX}bin/w32cross-treecasefix" "${IVTW_SDKPFX}"
