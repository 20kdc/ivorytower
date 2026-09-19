cross_install() {
	mkdir -p "$MESON_CROSS"
	cat > "$MESON_CROSS/$1"
	echo "  '$1' registered with Meson."
}
