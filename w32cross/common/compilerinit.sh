#!/bin/sh -e

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

connect clang-cl cl "-fuse-ld=w32cross-link $SDK_CL_ARGS"

# These wrappers use names consistent with the architecture names used throughout the SDK.
connect clang-cl cl-x86 "-fuse-ld=w32cross-link --target=i686-windows-msvc $SDK_CL_ARGS"
connect clang-cl cl-x64 "-fuse-ld=w32cross-link --target=x86_64-windows-msvc $SDK_CL_ARGS"
connect clang-cl cl-arm64 "-fuse-ld=w32cross-link --target=aarch64-windows-msvc $SDK_CL_ARGS"

connect llvm-ml ml ""
# note the LLD! llvm-link is something different
connect lld-link link ""
connect llvm-rc rc ""
connect llvm-cvtres cvtres ""
connect llvm-lib lib ""

# prepare utilities
clang++ common/fixerupper.cpp -o "${IVTW_SDKPFX}bin/w32cross-treecasefix"

cat <<EOF > "${IVTW_SDKPFX}activate"
PATH="$(readlink -f "${IVTW_SDKPFX}bin"):\$PATH"
EOF
chmod +x "${IVTW_SDKPFX}activate"
