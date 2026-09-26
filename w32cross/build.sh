#!/bin/sh -e

WINCROSS_DIR="`readlink -e "$0"`"
export WINCROSS_DIR="`dirname $WINCROSS_DIR`"

cd "$WINCROSS_DIR"

# This script will need rearrangement for any other SDK.
# Maybe it would be better to have different scripts for different arrangements, idk.

rm -f stages/sdk.sh
ln -s ../sdkcfg/10.0.19041.0.sh stages/sdk.sh

. stages/sdk.sh

./stages/0download.sh

rm -rf build target

./stages/1fixclangcl.sh
./stages/2isoextract.sh
./stages/3msiextract.sh
./stages/4fixsdk.sh
./stages/5vcruntime.sh

exit 1
