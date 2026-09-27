#!/bin/sh

. common/cbase.sh

# find which caused what
msiextract_all() {
	while read line; do
		echo
		echo "$line"
		echo
		msiextract -l "${IVTW_SDKPFX}build/isoextract/Installers/$line" -C build/msiextract
		echo
	done
}

sdk_isoextract
ls "${IVTW_SDKPFX}build/isoextract/Installers" | msiextract_all > dev_scavenger.log
