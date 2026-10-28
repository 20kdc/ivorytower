#!/bin/sh -e

cd "$(dirname "$(readlink -e "$0")")"

if [ "$W32CROSS_SDKID" = "" ]; then
	export W32CROSS_SDKID=w10
fi

common/compilerinit.sh

. common/cbase.sh

rm -rf tests/bin

echo "${IVTW_CLANG}"

do_test() {
	do_test_cfg="$1"
	shift
	do_test_out="$2"
	shift
	echo "build tests/bin/clang/$CURRENT_ARCH/${do_test_out}: clang_${do_test_cfg}"
	echo "	vcarch=${CURRENT_ARCH}"
}
# CL stuff
# /EHs /MD $args_cl /clang:-Wl,/demangle:no
# "/D_UNICODE"
# "/LD"

cat > "tests/gen.ninja" <<EOF
cl=${IVTW_CL}
clang=${IVTW_CLANG}
EOF
ninja -f "tests/01init.ninja"

# we can reasonably assume cl/clang 'compile the same'

run_test_sequence_inner() {
	echo "a_argsa"
	wine "tests/bin/$1/a_argsa.exe" "🛠"
	echo "a_argsw"
	wine "tests/bin/$1/a_argsw.exe" "🛠"
	echo "b_cppcrt"
	wine "tests/bin/$1/b_cppcrt.exe"
	echo "cxxdll"
	wine "tests/bin/$1/cxxdll.exe"
}

run_test_sequence() {
	run_test_sequence_inner "$1" | sed 's/\r//' > "tests/bin/$1/output.txt"
}

run_test_sequence cl_x86
run_test_sequence cl_x64

sha256sum "tests/output.ref.txt" "tests/bin/cl_x86/output.txt" "tests/bin/cl_x64/output.txt"
