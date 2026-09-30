#!/bin/sh -e

# IVTW_CL_ARGS etc. come from here
. common/cbase.sh

mkdir -p "${IVTW_SDKPFX}bin"

# clang-cl is only linked by version on at least Ubuntu 24.04 for clang-cl-18.
# Technically, we could rely on regular Clang and just pass GNU args only.
# However, this is likely to be 'mildly upsetting' to wrappers like Meson which we really need to convince to operate in MSVC mode for our faux MSVC toolchain.
# We don't want it passing -Xlinker ThingThatOnlyGNUWillAccept.
# We also need to -fuse=llvm-link as clang-cl-18 will default to trying to invoke a GNU linker with link.exe args, which fails.

# Due to all of this (and more!), we need to create these wrapper scripts for LLVM programs:
# w32cross-cl
# w32cross-ml
# w32cross-link
# w32cross-rc
# w32cross-cvtres
# w32cross-lib
# cl.exe
# link.exe

# We also need to consider https://github.com/llvm/llvm-project/blob/main/llvm/lib/WindowsDriver/MSVCPaths.cpp#L526
# (llvm::findVCToolChainViaEnvironment, telling LLVM we're using UCRT, etc).

# connect FULL SUB OPTS
connect() {
	ivtw_find_command "$1"
	cat <<EOF > "${IVTW_SDKPFX}bin/w32cross-$2"
#!/bin/sh -e
# W32Cross-generated tool configuration file.
W32CROSS_BINDIR="\$(dirname "\$(readlink -f "\$0")")"
# -fuse-ld=w32cross-link won't work if Clang can't find w32cross-link.
export PATH="$W32CROSS_BINDIR:$PATH"
W32CROSS_SDKROOT="\$(dirname "\$W32CROSS_BINDIR")"
# echo "$PATH"
# echo "W32CROSS_SDKROOT: \$W32CROSS_SDKROOT"
exec "$ivtw_find_command_result" $3 "\$@"
EOF
	chmod +x "${IVTW_SDKPFX}bin/w32cross-$2"
}
# SRC DST
xlink() {
	rm -f "${IVTW_SDKPFX}bin/$1"
	ln -s "w32cross-$1" "${IVTW_SDKPFX}bin/$2"
}
# COMPATNAME PATH
xlink_compat() {
	# Forced compiler autodetect without overrides sucks.
	# See: https://github.com/mesonbuild/meson/blob/3f1673ae45b8d6beb1631a76e1def0e8fb85d9d8/mesonbuild/compilers/detect.py
	# In order to dodge this, we provide the 'bin_compat' directory.
	# This directory is less safe to put into PATH, but it makes build systems see what they want to see.
	rm -f "${IVTW_SDKPFX}bin_compat/$1" "${IVTW_SDKPFX}bin_compat/$1.exe"
	mkdir -p "$(dirname "${IVTW_SDKPFX}bin_compat/$1")"
	ln -s "$2" "${IVTW_SDKPFX}bin_compat/$1"
	ln -s "$2" "${IVTW_SDKPFX}bin_compat/$1.exe"
}

# First, find the -fuse-ld linker.
# clang-cl pipelines linking-relevant sysroot stuff to the linker.
# This is good, because setting a 'proper' value for -fuse-ld= would probably have undesirable consequences.
# At the same time, Meson will invoke the linker independently (and with linker flags like /MACHINE).
# This means that we ABSOLUTELY need Meson to go through w32cross-link.
# Between this and its detection logic re: link and clang-cl, bin_compat is vital to running things.
ivtw_find_command "lld-link"
OUR_FUSELD="-fuse-ld=$ivtw_find_command_result"

connect clang-cl clang-cl "$OUR_FUSELD $IVTW_CL_ARGS"
xlink clang-cl cl
xlink_compat "cl" "../bin/w32cross-cl"

# These wrappers use names consistent with the architecture names used throughout the SDK.
connect clang-cl cl-x86 "$OUR_FUSELD --target=i686-windows-msvc $IVTW_CL_ARGS"
xlink_compat "x86/cl" "../../bin/w32cross-cl-x86"
connect clang-cl cl-x64 "$OUR_FUSELD --target=x86_64-windows-msvc $IVTW_CL_ARGS"
xlink_compat "x64/cl" "../../bin/w32cross-cl-x64"
connect clang-cl cl-arm64 "$OUR_FUSELD --target=aarch64-windows-msvc $IVTW_CL_ARGS"
xlink_compat "arm64/cl" "../../bin/w32cross-cl-arm64"
connect clang-cl clang-cl-x86 "$OUR_FUSELD --target=i686-windows-msvc $IVTW_CL_ARGS"
xlink_compat "x86/clang-cl" "../../bin/w32cross-clang-cl-x86"
connect clang-cl clang-cl-x64 "$OUR_FUSELD --target=x86_64-windows-msvc $IVTW_CL_ARGS"
xlink_compat "x64/clang-cl" "../../bin/w32cross-clang-cl-x64"
connect clang-cl clang-cl-arm64 "$OUR_FUSELD --target=aarch64-windows-msvc $IVTW_CL_ARGS"
xlink_compat "arm64/clang-cl" "../../bin/w32cross-clang-cl-arm64"

connect llvm-ml ml "$IVTW_ML_ARGS"
xlink_compat "ml" "../bin/w32cross-ml"
connect llvm-ml llvm-ml "$IVTW_ML_ARGS"
xlink_compat "llvm-ml" "../bin/w32cross-llvm-ml"
# note the LLD! llvm-link is something different
connect lld-link link "$IVTW_LINK_ARGS"
xlink_compat "link" "../bin/w32cross-link"
connect lld-link lld-link "$IVTW_LINK_ARGS"
xlink_compat "lld-link" "../bin/w32cross-lld-link"
# There's llvm-rc, but it's well known to be kind of a mess. :<
connect llvm-cvtres cvtres "$IVTW_CVTRES_ARGS"
xlink_compat "cvtres" "../bin/w32cross-cvtres"
connect llvm-cvtres llvm-cvtres "$IVTW_CVTRES_ARGS"
xlink_compat "llvm-cvtres" "../bin/w32cross-llvm-cvtres"
connect llvm-lib lib "$IVTW_LIB_ARGS"
xlink_compat "lib" "../bin/w32cross-lib"
connect llvm-lib llvm-lib "$IVTW_LIB_ARGS"
xlink_compat "llvm-lib" "../bin/w32cross-llvm-lib"

# prepare utilities
$CXX common/fixerupper.cpp -o "${IVTW_SDKPFX}bin/w32cross-treecasefix"

cat <<EOF > "${IVTW_SDKPFX}activate"
PATH="$(readlink -f "${IVTW_SDKPFX}bin"):\$PATH"
EOF
chmod +x "${IVTW_SDKPFX}activate"
