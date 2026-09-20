itsetup_decldep() {
	eval "`printf "arg_no_$1() { \n HAS_$1=0 \n } \n arg_force_$1() { \n HAS_$1=forced \n }"`"
}

HAS_distrobox="`distrobox --version 2> /dev/null`"
itsetup_decldep distrobox
HAS_meson="`meson --version 2> /dev/null`"
itsetup_decldep meson
HAS_wine="`wine --version 2> /dev/null`"
itsetup_decldep wine

ITSETUP_DEPS="distrobox meson wine"

# XDG_DATA_DIRS is multiple-dir, which is awkward, so just assume this is included. This is all we can do
if [ "$ITSETUP_MESON_SHARE" = "" ]; then
	ITSETUP_MESON_SHARE="$HOME/.local/share/meson"
fi
if [ "$ITSETUP_MESON_CROSS" = "" ]; then
	ITSETUP_MESON_CROSS="$ITSETUP_MESON_SHARE/cross"
fi

diagnostic() {
	itsetup_whatami

	echo "Available tooling:"
	echo
	for dep in $ITSETUP_DEPS; do
		itsetup_printvar "HAS_$dep"
	done
	echo
	echo "Other:"
	echo
	itsetup_printvar ITSETUP_MESON_SHARE
	itsetup_printvar ITSETUP_MESON_CROSS
	echo
}

arg_version() {
	diagnostic
	ITSETUP_EXIT_STATUS=0
}

arg_help() {
	itsetup_whatami
	echo "Command-line args:"
	echo " version: report version and exit"
	echo " help: show help and exit"
	echo " no_DEP: Forget we have given dep."
	echo " force_DEP: Assume we have given dep."
	echo " COMPONENT_off: turn on given component"
	echo " COMPONENT_on: turn on given component"
	echo ""
	echo "Dependencies:"
	for dep in $ITSETUP_DEPS; do
		echo " $dep"
	done
	echo ""
	echo "Components:"
	for arghelp_component in $ITSETUP_COMPONENTS; do
		eval "arghelp_compinfo=\"\$ITSETUP_COMPINFO_$arghelp_component\""
		echo " $arghelp_component: $arghelp_compinfo"
	done
	ITSETUP_EXIT_STATUS=0
}
