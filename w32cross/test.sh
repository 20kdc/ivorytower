#!/bin/sh -e

cd "$(dirname "$(readlink -e "$0")")"

if [ "$W32CROSS_SDKID" = "" ]; then
	export W32CROSS_SDKID=w10
fi

common/compilerinit.sh

. common/cbase.sh

rm -rf tests/bin tests/bin_cl
mkdir -p tests/bin
mkdir -p tests/bin_cl

do_test() {
	# echo "$1"
	test_file="$1"
	shift
	test_out="$1"
	shift
	args_cl="$1"
	shift
	args_clang="$1"
	shift
	if "$IVTW_CL" /EHs /MD $args_cl "/Fetests/bin_cl/$test_out" "tests/$test_file" ; then
		true
	else
		"$IVTW_CL" /c /FA1 /EHs /MD "/Fatests/bin_cl/$test_out.asm" "/Fotests/bin_cl/$test_out.obj" "tests/$test_file"
		false
	fi
	"$IVTW_CLANG_X64" -fms-runtime-lib=dll $args_clang -o "tests/bin/$test_out" "tests/$test_file"
}

do_test a_hello.c hello.exe "" ""
do_test a_args.c args_a.exe "" ""
do_test a_args.c args_w.exe "/D_UNICODE" ""
do_test b_constructors.cpp constructors.exe "" ""
do_test b_hellocpp.cpp hellocpp.exe "" ""
do_test c_cxxdll.cpp cxxdll.dll "/LD" "-shared"
do_test c_cxxdllexe.cpp cxxdll.exe "tests/bin_cl/cxxdll.lib" "tests/bin_cl/cxxdll.lib"
