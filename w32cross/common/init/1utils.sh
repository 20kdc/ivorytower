#!/bin/sh -e
. common/cbase.sh

# Various utilities. Some compiled, some templated in.

mkdir -p "${IVTW_SDKPFX}bin"

$CXX common/fixerupper.cpp -o "${IVTW_SDKPFX}bin/w32cross-treecasefix"

cat <<EOF > "${IVTW_SDKPFX}bin/w32cross-compat"
#!/bin/sh -e
# Calls a program in the bin_compat directory.
# This program exists to provide metadata to Meson.
# For more information, please see the following checks:
# https://github.com/mesonbuild/meson/blob/3e1c48717281b0c385a7aa87fb10ee2952bd421c/mesonbuild/compilers/detect.py#L228
# https://github.com/mesonbuild/meson/blob/3e1c48717281b0c385a7aa87fb10ee2952bd421c/mesonbuild/compilers/detect.py#L320
$IVTW_MACRO_WHEREAMI
cmd="\$W32CROSS_BINDIR/../bin_compat/\$1"
shift
exec "\$cmd" "\$@"
EOF
chmod +x "${IVTW_SDKPFX}bin/w32cross-compat"

cat <<EOF > "${IVTW_SDKPFX}activate"
PATH="$(readlink -f "${IVTW_SDKPFX}bin"):\$PATH"
EOF
chmod +x "${IVTW_SDKPFX}activate"
