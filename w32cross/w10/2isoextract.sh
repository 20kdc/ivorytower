#!/bin/sh -e

. common/cbase.sh

rm -rf build/isoextract
mkdir -p build/isoextract

7z x -obuild/isoextract "$ISO_PATH"
