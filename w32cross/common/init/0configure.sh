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

# First, find the -fuse-ld linker.
# clang-cl pipelines linking-relevant sysroot stuff to the linker.
# This is good, because setting a 'proper' value for -fuse-ld= would probably have undesirable consequences.
# At the same time, Meson will invoke the linker independently (and with linker flags like /MACHINE).
# This means that we ABSOLUTELY need Meson to go through w32cross-link.
# Between this and its detection logic re: link and clang-cl, bin_compat is vital to running things.
ivtw_find_command "lld-link"

OUR_FUSELD="-fuse-ld=$ivtw_find_command_result"
# For clang-cl, we need to forcibly inject /safeseh:no because it won't otherwise listen.
OUR_FUSELD_CL="-fuse-ld=\"w32cross-link\""
# this is stuff that is forcibly injected via that mechanism
OUR_FORCEDLINKARGS="/safeseh:no"
# Note: "/winsysroot X" works for clang-cl but not for lld-link.
# "/winsysroot:X" will result in prefixing ":" to everything.
OUR_WINSYSROOT="/winsysroot \"\$W32CROSS_SDKROOT/fakewinsysroot\" /winsdkver:10"

# -- clang-cl --

connect clang-cl clang-cl "$OUR_FUSELD_CL $OUR_WINSYSROOT -Xlinker/safeseh:no"
xlink clang-cl cl
xlink_compat "cl" "../bin/w32cross-cl"

# These wrappers use names consistent with the architecture names used throughout the SDK.
for arch in $IVTW_VCARCHS; do
	ivtw_vcarch_clangtarget "$arch"
	connect clang-cl "clang-cl-${arch}" "$OUR_FUSELD_CL --target=$ivtw_vcarch_clangtarget_result $OUR_WINSYSROOT"
	xlink "clang-cl-${arch}" "cl-${arch}"
	for clanginess in "" "clang-"; do
		xlink_compat "${arch}/${clanginess}cl" "../../bin/w32cross-clang-cl-${arch}"
	done
done

# -- clang-args --

gen_clangargs_lst_common() {
	echo "$OUR_FUSELD"
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
	for package in $SDK_PACKAGES; do
		echo "-L\${W32CROSS_SDKROOT}/$package/lib/$1"
	done
	# This has to be defined outside because clang-cl just won't listen anyway I guess.
	# See-also: OUR_FUSELD_CL, lld-link args
	# Rationale: lld-link: error: /safeseh: ../../../../VCRUNTIME140.dll is not compatible with SEH
	# (looks like the forwarder did it somehow)
	echo "-Wl,-safeseh:no"
	# The following are defined also in crt0_common.h and are described there.
	echo "-Wl,-defaultlib:kernel32"
	if [ "$1" = "x86" ]; then
		echo "-Wl,/alternatename:__chkstk=__alloca"
	elif [ "$1" = "x64" ]; then
		echo "-Wl,/alternatename:__chkstk=___chkstk_ms"
	fi
	echo "-Wl,/alternatename:__alldiv=___divdi3"
	echo "-Wl,/alternatename:__aulldiv=___udivdi3"
	echo "-Wl,/alternatename:__allrem=___moddi3"
	echo "-Wl,/alternatename:__aullrem=___umoddi3"
	echo "-Wl,/alternatename:__allmul=___muldi3"
	echo "-Wl,/alternatename:??_7type_info@@6B@.1=??_7type_info@@6B@"
}

rm -f "${IVTW_SDKPFX}etc/packages"
for package in $SDK_PACKAGES; do
	echo "$package" >> "${IVTW_SDKPFX}etc/packages"
done

mkdir -p "${IVTW_SDKPFX}etc/clang-args"
mkdir -p "${IVTW_SDKPFX}etc/windres-args"
gen_clangargs_lst_common > "${IVTW_SDKPFX}etc/clang-args/any.lst"
gen_clangargs_lst_common > "${IVTW_SDKPFX}etc/windres-args/any.lst"

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
	set_our_clangargs < "${IVTW_SDKPFX}etc/clang-args/any.lst"
	connect "clang${plusplus}" "clang${plusplus}" "$OUR_CLANGARGS"
	xlink_compat "clang${plusplus}" "../bin/w32cross-clang${plusplus}"

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

# note the LLD! llvm-link is something different
connect lld-link link "$OUR_FORCEDLINKARGS"
xlink_compat "link" "../bin/w32cross-link"
connect lld-link lld-link "$OUR_FORCEDLINKARGS"
xlink_compat "lld-link" "../bin/w32cross-lld-link"
