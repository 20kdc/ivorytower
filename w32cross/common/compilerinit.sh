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
# This will be the root and thus 'w32cross-conf' is required.
# We also-also pretend to be an "old style" SDK even though we use VC2019.
# This is because the new SDKs contain spaces and we don't want brittleness as a result of that.

# connect FULL SUB OPTS
connect() {
	ivtw_find_command "$1"
	cat <<EOF > "${IVTW_SDKPFX}bin/w32cross-$2"
#!/bin/sh -e
exec "$ivtw_find_command_result" "\$@"
EOF
	chmod +x "${IVTW_SDKPFX}bin/w32cross-$2"
}

connect clang-cl cl ""
connect llvm-ml ml ""
# note the LLD! llvm-link is something different
connect lld-link link ""
connect llvm-rc rc ""
connect llvm-cvtres cvtres ""
connect llvm-lib lib ""

# Create w32cross-conf.
absolute_target="`readlink -f target`"
cat <<EOF > "${IVTW_SDKPFX}bin/w32cross-conf"
#!/bin/sh -e
# W32Cross-generated configuration file.
echo export VCINSTALLDIR=\"$absolute_target\"
EOF
chmod +x "${IVTW_SDKPFX}bin/w32cross-conf"
