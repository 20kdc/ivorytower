#!/bin/sh
# This file is deployed into the SDK at SDKROOT/etc/sdkcommon.sh

# just in case any w32cross- wrappers are needed
# in theory this should get deduplicated, oh well
export PATH="$W32CROSS_BINDIR:$PATH"
# echo "\$PATH"
# echo "W32CROSS_SDKROOT: \$W32CROSS_SDKROOT"

ivtw_args2lines() {
	while [ "$#" != 0 ]; do
		echo "$1"
		shift
	done
}

ivtw_lines2args() {
	if read A; then
		if read B; then
			if read C; then
				if read D; then
					ivtw_lines2args "$@" "$A" "$B" "$C" "$D"
				else
					ivtw_lines2args "$@" "$A" "$B" "$C"
				fi
			else
				ivtw_lines2args "$@" "$A" "$B"
			fi
		else
			ivtw_lines2args "$@" "$A"
		fi
	else
		# command is ready
		exec "$@"
	fi
}
