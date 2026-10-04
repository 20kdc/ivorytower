#!/bin/sh -e

# IVTW_CL_ARGS etc. come from here
. common/cbase.sh

init_stage() {
	echo -n "$1 "
	"common/init/$1.sh"
}

echo -n "compilerinit: "
init_stage 0configure
init_stage 1utils
init_stage 2meson
init_stage 3compiler_rt
init_stage 4winsysroot
echo "[OK]"
