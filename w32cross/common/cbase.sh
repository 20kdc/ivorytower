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

# SDK build tools (for w32cross-treecasefix and any future similar tools)

if [ "$CXX" = "" ]; then
	CXX="c++"
fi

# Core directory variables.
# These should be based on prefixes to prevent 'root -rf risk'.
IVTW_DLPFX="downloaded/"
IVTW_SDKPFX="sdk_$W32CROSS_SDKID/"

IVTW_CL="${IVTW_SDKPFX}bin/w32cross-cl"
IVTW_LIB="${IVTW_SDKPFX}bin/w32cross-lib"

# -- SDK variable defaults --

SDK_CL_ARGS=""
SDK_LINK_ARGS=""

# -- Core Utilities --

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

# -- Command Discovery --

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

# -- Disclaimer --

# PATH TEXT
ivtw_disclaimer_dl() {
	echo " $1 : $2"
	if [ -e "$1" ]; then
		echo "  (already downloaded, will be skipped)"
	fi
}

# -- Bootstrapping --

# ivtw_import_defs PACKAGE ARCH OUTPATH
ivtw_import_defs() {
	for ivtw_import_defs_def in `ls "defs/$1/$2"`; do
		"$IVTW_LIB" "/machine:$2" "/def:defs/$1/$2/$ivtw_import_defs_def" "/out:$3/$(echo "$ivtw_import_defs_def" | sed "s/def$/lib/g")"
	done
}

. "$W32CROSS_SDKID/0sdkdef.sh"

# -- SDK-derived IVTW variables --

# Note: "/winsysroot X" works for clang-cl but not for lld-link.
# "/winsysroot:X" will result in prefixing ":" to everything.
# This sort of thing is why common (and thus compilersetup) handles this stuff, not SDK.
IVTW_CL_ARGS="/winsysroot \"\$W32CROSS_SDKROOT\" $SDK_CL_ARGS"
IVTW_ML_ARGS=""
IVTW_LINK_ARGS="\"/winsysroot:\$W32CROSS_SDKROOT\" $SDK_LINK_ARGS"
IVTW_RC_ARGS=""
IVTW_CVTRES_ARGS=""
IVTW_LIB_ARGS=""
