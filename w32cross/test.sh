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
include tests/01init.ninja
EOF
ninja -f "tests/gen.ninja"
echo BE SURE TO RUN tests/bin/cl/x86/constructors.exe
