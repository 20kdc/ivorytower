#ITSETUP_COMPONENTS="test narwhal"
#ITSETUP_COMPINFO_test="Prototype"
#ITSETUP_COMPINFO_narwhal="Other Prototype"

ITSETUP_COMPONENTS="all"
ITSETUP_COMPINFO_all="Shorthand for enabling/disabling all components."

comp_all() {
	true
}

arg_all_on() {
	for argall_component in $ITSETUP_COMPONENTS; do
		eval "ITSETUP_COMP_$argall_component=1"
	done
	ITSETUP_COMP_all=0
}
arg_all_off() {
	for argall_component in $ITSETUP_COMPONENTS; do
		eval "ITSETUP_COMP_$argall_component=0"
	done
}

# assumes comp_narwhal()
# creates arg_narwhal_off and arg_narwhal_on
itsetup_declcomp() {
	ITSETUP_COMPONENTS="$ITSETUP_COMPONENTS $1"
	eval "ITSETUP_COMPINFO_$1=\"$2\""
	eval "`printf "arg_$1_off() { \n ITSETUP_COMP_$1=0 \n } \n arg_$1_on() { \n ITSETUP_COMP_$1=1 \n }"`"
}

# itsetup_declcomp "narwhal" "Prototypical narwhal"
ITSETUP_COMP_narwhal=0
comp_narwhal() {
	echo "Narwhal component runs"
}

itsetup_runner() {
	for runner_component in $ITSETUP_COMPONENTS; do
		eval "runner_compstate=\"\$ITSETUP_COMP_$runner_component\""
		if [ "$runner_compstate" = "1" ]; then
			eval "runner_compinfo=\"\$ITSETUP_COMPINFO_$runner_component\""
			echo "$runner_compinfo:"
			eval "\"comp_$runner_component\""
		fi
	done
}
