#!/bin/sh

. common/cbase.sh

# find which caused what
msiextract_all() {
	while read line; do
		echo
		echo "$line"
		echo
		msiextract "build/isoextract/Installers/$line" -C build/msiextract
		echo
	done
}
ls build/isoextract/Installers | msiextract_all > dev_scavenger.log
