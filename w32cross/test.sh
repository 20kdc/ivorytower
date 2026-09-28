#!/bin/sh -e

cd "$(dirname "$(readlink -e "$0")")"

if [ "$W32CROSS_SDKID" = "" ]; then
	export W32CROSS_SDKID=w10
fi

. common/cbase.sh

PATH="$(readlink -f "${IVTW_SDKPFX}bin"):$PATH"

rm -rf tests/bin
mkdir -p tests/bin

do_test() {
	# echo "$1"
	test_file="$1"
	shift
	test_out="$1"
	shift
	if w32cross-cl /EHs /MD "$@" "/Fetests/bin/$test_out" "tests/$test_file" ; then
		true
	else
		w32cross-cl /c /FA1 /EHs /MD "/Fatests/bin/$test_out.asm" "/Fotests/bin/$test_out.obj" "tests/$test_file"
		false
	fi
}

do_test a_hello.c hello.exe
do_test a_args.c args.exe
do_test b_constructors.cpp constructors.exe
do_test b_hellocpp.cpp hellocpp.exe
do_test c_cxxdll.cpp cxxdll.dll /LD
do_test c_cxxdllexe.cpp cxxdll.exe tests/bin/cxxdll.lib
