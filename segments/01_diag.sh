
it_diag_var() {
	eval "echo \"  $1=\$$1\""
}

HAS_DISTROBOX="`distrobox --version 2> /dev/null`"
HAS_MESON="`meson --version 2> /dev/null`"
HAS_WINE="`wine --version 2> /dev/null`"

# XDG_DATA_DIRS is multiple-dir, which is awkward, so just assume this is included. This is all we can do
MESON_SHARE="$HOME/.local/share/meson"
MESON_CROSS="$MESON_SHARE/cross"

diagnostic() {
	echo
	echo " Ivory Tower setup program"
	echo

	it_diag_var ITSETUP_VER
	it_diag_var ITSETUP_WHERE
	it_diag_var ITSETUP_DIR

	echo
	echo " Available tooling:"
	echo
	it_diag_var HAS_DISTROBOX
	it_diag_var HAS_MESON
	it_diag_var HAS_WINE
	echo
	echo " Other:"
	echo
	it_diag_var MESON_SHARE
	it_diag_var MESON_CROSS
	echo
}
