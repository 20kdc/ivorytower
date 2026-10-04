#!/bin/sh -e

. common/cbase.sh

# -- Bootstrap crossfiles --

mkdir -p "${IVTW_SDKPFX}etc/meson_bootstrap"
# We just do these manually
bootstrap_crossfile_gen() {

cat <<EOF > "${IVTW_SDKPFX}etc/meson_bootstrap/$1"
[binaries]
c = 'w32cross-clang-$1'
cpp = 'w32cross-clang++-$1'
ar = ['w32cross-compat', 'llvm-lib']
windres = 'w32cross-windres-$1'
strip = 'w32cross-strip'
[host_machine]
system = 'windows'
subsystem = 'windows'
kernel = 'nt'
cpu_family = '$2'
cpu = '$3'
endian = 'little'
[properties]
needs_exe_wrapper = true
[built-in options]
# cpp_eh = 's' # have to pass manually
cpp_args = ['-fexceptions', '-fcxx-exceptions']
optimization = 's'
EOF
}

bootstrap_crossfile_gen x86 x86 i686
bootstrap_crossfile_gen x64 x86_64 x86_64
bootstrap_crossfile_gen arm64 aarch64 aarch64
