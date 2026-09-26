#!/bin/sh -e

. stages/sdk.sh

rm -rf build/msiextract target
mkdir -p build/msiextract target

sdk_msiextract

rm -r build/msiextract
