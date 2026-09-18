diagnostic

cross_install() {
	mkdir -p "$MESON_CROSS"
	cat > "$MESON_CROSS/$1"
	echo "  $1"
}

# -- Windows strategy --

gen_mingw_wineifwehaveit() {
	if [ "$HAS_WINE" != "" ]; then
		echo "exe_wrapper = 'wine'"
	fi
}

gen_mingw_nostl() {
	cat <<EOF
[binaries]
c = '$2-w64-mingw32-gcc-win32'
cpp = '$2-w64-mingw32-g++-win32'
ar = '$2-w64-mingw32-ar'
windres = '$2-w64-mingw32-windres'
strip = '$2-w64-mingw32-strip'
EOF
	gen_mingw_wineifwehaveit
cat <<EOF

[properties]
needs_exe_wrapper = true

[host_machine]
system = 'windows'
cpu_family = '$1'
cpu = '$2'
endian = 'little'

[built-in options]
c_args = ['-fno-exceptions']
c_link_args = ['-nostdlib++', '-static-libgcc']
cpp_args = ['-nostdinc++', '-fno-rtti', '-fno-exceptions']
cpp_link_args = ['-nostdlib++', '-static-libgcc']
EOF
}

gen_mingw_staticstl() {
	cat <<EOF
[binaries]
c = '$2-w64-mingw32-gcc-win32'
cpp = '$2-w64-mingw32-g++-win32'
ar = '$2-w64-mingw32-ar'
windres = '$2-w64-mingw32-windres'
strip = '$2-w64-mingw32-strip'
EOF
	gen_mingw_wineifwehaveit
cat <<EOF

[properties]
needs_exe_wrapper = true

[host_machine]
system = 'windows'
cpu_family = '$1'
cpu = '$2'
endian = 'little'

[built-in options]
c_args = []
c_link_args = ['-static-libgcc']
cpp_args = []
cpp_link_args = ['-static-libgcc', '-static-libstdc++']
EOF
}

# -- Linux strategy --

gen_distrobox() {
cat <<EOF
[binaries]
c = [ 'distrobox', 'enter', '$1', '--', 'gcc' ]
cpp = [ 'distrobox', 'enter', '$1', '--', 'g++' ]
ar = [ 'distrobox', 'enter', '$1', '--', 'ar' ]
strip = [ 'distrobox', 'enter', '$1', '--', 'strip' ]

[host_machine]
system = 'linux'
cpu_family = 'x86'
cpu = 'i686'
endian = 'little'

[built-in options]
cpp_args = []
cpp_link_args = []
EOF
}

# -- macOS strategy --

# -- index --

echo " Installing cross files..."
echo
# windows
gen_mingw_nostl "x86" "i686" | cross_install "ivt_wx32_nostl"
gen_mingw_nostl "x86_64" "x86_64" | cross_install "ivt_wx64_nostl"
gen_mingw_staticstl "x86" "i686" | cross_install "ivt_wx32_staticstl"
gen_mingw_staticstl "x86_64" "x86_64" | cross_install "ivt_wx64_staticstl"
# linux
gen_distrobox "scout-i386" | cross_install "ivt_lgx32_scout"
gen_distrobox "scout" | cross_install "ivt_lgx64_scout"
gen_distrobox "sniper" | cross_install "ivt_lgx64_sniper"
# mac
#gen_zigcc "x86_64-macos" | cross_install "ivt_mx64"
#gen_zigcc "aarch64-macos" | cross_install "ivt_ma64"
echo
