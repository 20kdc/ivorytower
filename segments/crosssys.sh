cross_install() {
	mkdir -p "$ITSETUP_MESON_CROSS"
	cat > "$ITSETUP_MESON_CROSS/$1"
	echo "  '$1' registered with Meson."
}

cross_meson_mach_lgx32() {
cat <<EOF
[host_machine]
system = 'linux'
cpu_family = 'x86'
cpu = 'i686'
endian = 'little'
EOF
}

# cross_meson_mach linux i686
cross_meson_mach() {
if [ "$2" = "i686" ]; then
	cross_meson_mach_cpu_family="x86"
elif [ "$2" = "x86_64" ]; then
	cross_meson_mach_cpu_family="x86_64"
else
	echo "OOPS" > /dev/stderr
	exit 1
fi
cat <<EOF
[host_machine]
system = '$1'
cpu_family = '$cross_meson_mach_cpu_family'
cpu = '$3'
endian = 'little'
EOF
}
