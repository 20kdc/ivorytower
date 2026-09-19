itsetup_declcomp "scout" "'scout' SteamRT Distrobox (most/all x86_64 glibc Linuxes)"
itsetup_declcomp "scout_i386" "'scout-i386' SteamRT Distrobox (most/all x86 glibc Linuxes)"
itsetup_declcomp "sniper" "'sniper' SteamRT Distrobox (modern x86_64 glibc Linuxes)"

ITSETUP_COMP_scout=0
ITSETUP_COMP_scout_i386=0
ITSETUP_COMP_sniper=0
if [ "$HAS_distrobox" != "" ]; then
	ITSETUP_COMP_scout=1
	# don't default to enabling sniper or scout-i386.
	# sniper is kind of useless for portability (anything in scout that doesn't run on your machine which does work in sniper will break tomorrow)
	# and scout-i386 is like 900 extra MB and there's enough arguments against it to make it hard to justify.
fi

distrobox_prepare() {
	if [ "$HAS_distrobox" = "" ]; then
		echo "Component that requires distrobox selected but distrobox is not installed."
		echo "The command that would be run is: distrobox create -i \"$1\" \"$2\""
	else
		distrobox create -i "$1" "$2"
	fi
}

distrobox_genbinaries() {
cat <<EOF
[binaries]
c = [ 'distrobox', 'enter', '$1', '--', 'gcc' ]
cpp = [ 'distrobox', 'enter', '$1', '--', 'g++' ]
ar = [ 'distrobox', 'enter', '$1', '--', 'ar' ]
strip = [ 'distrobox', 'enter', '$1', '--', 'strip' ]
EOF
}

distrobox_gencomp() {
distrobox_genbinaries "$1"
cross_meson_mach linux "$2"
cat <<EOF

[built-in options]
cpp_args = []
cpp_link_args = []
EOF
}

comp_scout() {
	distrobox_prepare registry.gitlab.steamos.cloud/steamrt/scout/sdk scout
	distrobox_gencomp "scout" "x86_64" | cross_install "ivt_lgx64_scout"
}

comp_scout_i386() {
	distrobox_prepare registry.gitlab.steamos.cloud/steamrt/scout/sdk/i386 scout-i386
	distrobox_gencomp "scout-i386" "i686" | cross_install "ivt_lgx32_scout"
}

comp_sniper() {
	distrobox_prepare registry.gitlab.steamos.cloud/steamrt/sniper/sdk sniper
	distrobox_gencomp "sniper" "x86_64" | cross_install "ivt_lgx64_sniper"
}
