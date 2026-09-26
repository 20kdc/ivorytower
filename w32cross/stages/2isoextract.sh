#!/bin/sh -e

. stages/sdk.sh

rm -rf build/isoextract
mkdir -p build/isoextract

7z x -obuild/isoextract "$ISO_PATH"
