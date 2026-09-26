#!/bin/sh -e

WINCROSS_DIR="`readlink -e "$0"`"
export WINCROSS_DIR="`dirname $WINCROSS_DIR`"

cd "$WINCROSS_DIR"

# This script will need rearrangement for any other SDK.
# Maybe it would be better to have different scripts for different arrangements, idk.

rm -f stages/sdk.sh
ln -s ../sdkcfg/10.0.19041.0.sh stages/sdk.sh

. stages/sdk.sh

./stages/download.sh

rm -rf build target

./stages/isoextract.sh
./stages/msiextract.sh
./stages/fixerupper.sh
./stages/fixclangcl.sh

exit 1
