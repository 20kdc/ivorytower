#!/bin/sh -e

cd "$(dirname "$(readlink -e "$0")")"

if [ "$W32CROSS_SDKID" = "" ]; then
	export W32CROSS_SDKID=w10
fi

common/compilerinit.sh

. common/cbase.sh

rm -rf tests/bin tests/bin_cl

CURRENT_ARCH=""

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
	if "${IVTW_CL}-${CURRENT_ARCH}" /EHs /MD $args_cl /clang:-Wl,/demangle:no "/Fetests/bin/cl/$CURRENT_ARCH/$test_out" "tests/$test_file" ; then
		true
	else
		echo "cl $CURRENT_ARCH"
		"${IVTW_CL}-${CURRENT_ARCH}" /c /FA1 /EHs /MD "/Fatests/bin/cl/$CURRENT_ARCH/$test_out.asm" "/Fotests/bin/cl/$CURRENT_ARCH/$test_out.obj" "tests/$test_file"
		false
	fi
	"${IVTW_CLANG}-${CURRENT_ARCH}" -fms-runtime-lib=dll $args_clang -Wl,/demangle:no -o "tests/bin/clang/$CURRENT_ARCH/$test_out" "tests/$test_file"
}

for CURRENT_ARCH in $SDK_VCARCHS; do
	echo "tests in arch $CURRENT_ARCH"
	mkdir -p "tests/bin/cl/$CURRENT_ARCH"
	mkdir -p "tests/bin/clang/$CURRENT_ARCH"
	do_test a_hello.c hello.exe "" ""
	do_test a_args.c args_a.exe "" ""
	do_test a_args.c args_w.exe "/D_UNICODE" ""
	do_test b_constructors.cpp constructors.exe "" ""
	do_test b_hellocpp.cpp hellocpp.exe "" ""
	do_test c_cxxdll.cpp cxxdll.dll "/LD" "-shared"
	do_test c_cxxdllexe.cpp cxxdll.exe "tests/bin/cl/$CURRENT_ARCH/cxxdll.lib" "tests/bin/cl/$CURRENT_ARCH/cxxdll.lib"
	do_test d_micro_fbxcommon.cpp d_micro_fbxcommon.dll "/LD" "-shared"
	do_test z_endlesschamber.cpp z_endlesschamber.dll "/LD" "-shared"
done
