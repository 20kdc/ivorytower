#!/bin/sh -e

cd "$(dirname "$(readlink -e "$0")")"

if [ "$W32CROSS_SDKID" = "" ]; then
	export W32CROSS_SDKID=w10
fi

. common/cbase.sh

PATH="$(readlink -f "${IVTW_SDKPFX}bin"):$PATH"

rm -rf tests/bin
mkdir -p tests/bin

do_test_c() {
	w32cross-cl "tests/$1.c" "/Fetests/bin/$1.exe" ucrt.lib kernel32.lib
}

do_test_cpp() {
	w32cross-cl "tests/$1.cpp" "/Fetests/bin/$1.exe" ucrt.lib kernel32.lib
}

do_test_c hello
do_test_c args
do_test_cpp hellocpp
