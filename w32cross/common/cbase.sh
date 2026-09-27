# All common definitions for W32Cross setup.

# -- NOTICE ON LAYOUT --
# Internal variables are marked IVTW_.
# castlebase functions are marked ivtw_.
# Functions defined in SDK config are marked sdk_.

if [ "$W32CROSS_SDKID" = "" ]; then
	echo "common/castlebase.sh: Requires W32CROSS_SDKID"
	exit 1
elif [ ! -e "$W32CROSS_SDKID/0sdkdef.sh" ]; then
	echo "common/castlebase.sh: Could not find 0sdkdef.sh."
	echo "Possible causes:"
	echo "* SDK '$W32CROSS_SDKID' is not a known SDK synthesis chain."
	echo "* Script may have been run with incorrect CD."
fi

# Core directory variables.
# These should be based on prefixes to prevent 'root -rf risk'.
IVTW_DLPFX="downloaded/"
IVTW_SDKPFX="sdk_$W32CROSS_SDKID/"

# Runs a stage script.
ivtw_stage() {
	echo " - $1 -"
	"$W32CROSS_SDKID/$1.sh"
}

ivtw_get_path() {
	# We do a really silly trick here to get each PATH entry.
	# This is so that PATH entries with spaces are respected.
	whereis -bl | grep ^bin | sed "s/^[^:]*: //"
}

# ivtw_get_candidates_inner PATTERN
ivtw_get_candidates_inner() {
	while read ivtw_get_candidates_inner_candidate; do
		# this intentionally returns just 'raw command names'
		ls "$ivtw_get_candidates_inner_candidate" | grep "$1"
	done
}

# ivtw_get_candidates PATTERN
ivtw_get_candidates() {
	ivtw_get_path | ivtw_get_candidates_inner "$1" | sort -u
}

# ivtw_find_command PATTERN
# returns in ivtw_find_command_result
ivtw_find_command() {
	ivtw_find_command_result="$(ivtw_get_candidates "$1" | head -n 1)"
}

. "$W32CROSS_SDKID/0sdkdef.sh"
