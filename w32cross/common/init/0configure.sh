#!/bin/sh -e

# IVTW_CL_ARGS etc. come from here
. common/cbase.sh

mkdir -p "${IVTW_SDKPFX}bin"
mkdir -p "${IVTW_SDKPFX}etc"

# clang-cl is only linked by version on at least Ubuntu 24.04 for clang-cl-18.
# We have to thus find by version.
# We also need both clang-cl and regular clang working because of mesonbuild/meson#11180.
# We ALSO need to get clang-cl to actually work with our fancy new custom layout.
# Simply put, we are in hell.

# connect FULL SUB OPTS
connect() {
	ivtw_find_command "$1"
	cat <<EOF > "${IVTW_SDKPFX}bin/w32cross-$2"
#!/bin/sh -e
# W32Cross-generated tool configuration file.
$IVTW_MACRO_WHEREAMI
# just in case any w32cross- wrappers are needed
export PATH="\$W32CROSS_BINDIR:\$PATH"
W32CROSS_SDKROOT="\$(dirname "\$W32CROSS_BINDIR")"
# echo "\$PATH"
# echo "W32CROSS_SDKROOT: \$W32CROSS_SDKROOT"

exec "$ivtw_find_command_result" $3 "\$@"
EOF
	chmod +x "${IVTW_SDKPFX}bin/w32cross-$2"
}
# SRC DST
xlink() {
	rm -f "${IVTW_SDKPFX}bin/w32cross-$2"
	ln -s "w32cross-$1" "${IVTW_SDKPFX}bin/w32cross-$2"
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

# -- w32cross-link --

# These args are forcibly injected into *ALL* links.
# That is, they are installed in w32cross-link, which we use for all links.
#
# This is for a number of reasons:
# 1. clang-cl can be a pain in regards to getting it to actually listen to link args.
# 2. Meson does not understand `-Xlinker` and will 'transparently reorder' things.
#    Meanwhile the subsystem command contains a comma so we can't use `-Wl,`
#
# Rationale for each arg:
# /safeseh:no : lld-link: error: /safeseh: ../../../../VCRUNTIME140.dll is not compatible with SEH
# /subsystem:default,4.0 : LLVM has bad defaults, this enables XP support
OUR_FORCEDLINKARGS="/safeseh:no /subsystem:default,4.0"

# note the LLD! llvm-link is something different
connect lld-link link "$OUR_FORCEDLINKARGS"
xlink_compat "link" "../bin/w32cross-link"
connect lld-link lld-link "$OUR_FORCEDLINKARGS"
xlink_compat "lld-link" "../bin/w32cross-lld-link"

# -- clang args --

# As of IvoryTower preview2b (preview2 ran into a critical problem with Meson -Xlinker I knew of but forgot about), we need to run ALL links through w32cross-link.
# Also --ld-path is a lie (it was completely ignored in testing) and Santa isn't real.
OUR_FUSELD="-fuse-ld=w32cross-link"

# Some notes:
# * Since we do PATH injection, we don't need to provide the full path.
#   This is good, because clang-ld does not know what --ld-path is.
# * we inject /MD for the same reason we inject -fms-runtime-lib=dll on Clang
#   * we simply can't support anything else
#   * we can't trust all build systems to make this easy
#   * especially for clang interface (not clang-cl), we may need to pretend to be MinGW in some cases
# Note: "/winsysroot X" works for clang-cl but not for lld-link.
# "/winsysroot:X" will result in prefixing ":" to everything.
OUR_COMMONCLANGCLARGS="$OUR_FUSELD /winsysroot \"\$W32CROSS_SDKROOT/fakewinsysroot\" /winsdkver:10 /MD"

# -- clang-cl --

connect clang-cl clang-cl "$OUR_COMMONCLANGCLARGS /MD"
xlink clang-cl cl
xlink_compat "cl" "../bin/w32cross-cl"

# These wrappers use names consistent with the architecture names used throughout the SDK.
for arch in $IVTW_VCARCHS; do
	ivtw_vcarch_clangtarget "$arch"
	connect clang-cl "clang-cl-${arch}" "--target=$ivtw_vcarch_clangtarget_result $OUR_COMMONCLANGCLARGS"
	xlink "clang-cl-${arch}" "cl-${arch}"
	for clanginess in "" "clang-"; do
		xlink_compat "${arch}/${clanginess}cl" "../../bin/w32cross-clang-cl-${arch}"
	done
done

# -- clang-args --

gen_clangargs_lst_common() {
	echo "$OUR_FUSELD"
	# We can't provide -Wl, without causing warnings.
	# See gen_clangargs_lst_ld.
	for package in $SDK_PACKAGES; do
		echo "-isystem"
		echo "\${W32CROSS_SDKROOT}/$package/include"
	done
	echo "-fms-runtime-lib=dll"
}
gen_windres_lst_common() {
	for package in $SDK_PACKAGES; do
		echo "--include-dir=\${W32CROSS_SDKROOT}/$package/include"
	done
}
gen_clangargs_lst_arch() {
	ivtw_vcarch_clangtarget "$1"
	echo "--target=$ivtw_vcarch_clangtarget_result"
	gen_clangargs_lst_common
}
gen_windres_lst_arch() {
	ivtw_vcarch_clangtarget "$1"
	echo "--target=$ivtw_vcarch_clangtarget_result"
	gen_windres_lst_common
}
gen_clangargs_lst_ld() {
	gen_clangargs_lst_arch "$1"
	# https://github.com/llvm/llvm-project/blob/642daaf27dd0cda7127d35bad1a3fe705a267918/clang/lib/Driver/ToolChains/MSVC.cpp#L125
	# This *really* messes things up for no good reason. Luckily, we only support one configuration.
	# So we can choose to just solely support that one configuration.
	echo "-nostartfiles"
	echo "-Wl,-defaultlib:msvcrt"
	echo "-Wl,-defaultlib:msvcprt"
	echo "-Wl,-defaultlib:oldnames"
	for gcll_package in $SDK_PACKAGES; do
		echo "-L\${W32CROSS_SDKROOT}/$gcll_package/lib/$1"
	done
	# The following are defined also in crt0_common.h and are described there.
	echo "-Wl,-defaultlib:kernel32"
	if [ "$1" = "x86" ]; then
		echo "-Wl,/alternatename:__chkstk=__alloca"
	elif [ "$1" = "x64" ]; then
		echo "-Wl,/alternatename:__chkstk=___chkstk_ms"
	fi
}

rm -f "${IVTW_SDKPFX}etc/packages"
for package in $SDK_PACKAGES; do
	echo "$package" >> "${IVTW_SDKPFX}etc/packages"
done

mkdir -p "${IVTW_SDKPFX}etc/clang-args"
mkdir -p "${IVTW_SDKPFX}etc/windres-args"

gen_windres_lst_common "$arch" > "${IVTW_SDKPFX}etc/windres-args/any.lst"

for arch in $IVTW_VCARCHS; do
	gen_clangargs_lst_arch "$arch" > "${IVTW_SDKPFX}etc/clang-args/$arch.lst"
	gen_clangargs_lst_ld "$arch" > "${IVTW_SDKPFX}etc/clang-args/$arch.ld.lst"
	gen_windres_lst_arch "$arch" > "${IVTW_SDKPFX}etc/windres-args/$arch.lst"
done

# -- clang --

OUR_CLANGARGS=""

set_our_clangargs() {
	OUR_CLANGARGS=""
	while read line; do
		OUR_CLANGARGS="$OUR_CLANGARGS \"$line\""
	done
}

for plusplus in "" "++"; do
	connect "clang${plusplus}" "clang${plusplus}" ""
	xlink_compat "clang${plusplus}" "../bin/w32cross-clang${plusplus}"

	# per-arch ('ready to use')
	for arch in $IVTW_VCARCHS; do
		set_our_clangargs < "${IVTW_SDKPFX}etc/clang-args/$arch.ld.lst"
		# Disable -Wno-unused-command-line-argument because we attach linker args which may not be used.
		connect "clang${plusplus}" "clang${plusplus}-${arch}" "-Wno-unused-command-line-argument $OUR_CLANGARGS"
		xlink_compat "${arch}/clang${plusplus}" "../../bin/w32cross-clang${plusplus}-${arch}"
	done
done

# -- windres --

for llvminess in "" "llvm-"; do
	set_our_clangargs < "${IVTW_SDKPFX}etc/windres-args/any.lst"
	connect "llvm-windres" "${llvminess}windres" "$OUR_CLANGARGS"
	xlink_compat "${llvminess}windres" "../bin/w32cross-${llvminess}windres"

	for arch in $IVTW_VCARCHS; do
		set_our_clangargs < "${IVTW_SDKPFX}etc/windres-args/$arch.lst"
		connect "llvm-windres" "${llvminess}windres-${arch}" "$OUR_CLANGARGS"
		xlink_compat "${arch}/${llvminess}windres" "../../bin/w32cross-${llvminess}windres-${arch}"
	done
done

# -- llvm assorted --

# There's llvm-rc, but it's well known to be kind of a mess. :<
for verb in "ml" "cvtres" "lib" "strip"; do
	connect "llvm-${verb}" "llvm-${verb}" ""
	xlink_compat "llvm-${verb}" "../bin/w32cross-llvm-${verb}"
	xlink "llvm-${verb}" "${verb}"
	xlink_compat "${verb}" "../bin/w32cross-${verb}"
done
