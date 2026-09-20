# cross_install platform disposition_variant
# Auto-prepends platform declarator (i.e. 'ivt_lgx64_' and host_machine INI).
# Note that this is the 'bare bones' mode which only handles the host_machine definition by itself.
# When possible, wrapping code should be used which generates all STL dispositions.
cross_install() {
	mkdir -p "$ITSETUP_MESON_CROSS"
	cat "$ITSETUP_DIR/res/meson_host/$1.ini" - > "$ITSETUP_MESON_CROSS/ivt_$1_$2"
	echo "  'ivt_$1_$2' registered with Meson."
}
