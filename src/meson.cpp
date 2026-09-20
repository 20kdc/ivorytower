#include <stdlib.h>
#include <stdio.h>

#include "iblis.h"
#include "meson.h"

iblis::CString findMesonCrossPath() {
	const char * home = getenv("HOME");
	if (!home) {
		puts("missing HOME environment variable");
		exit(1);
	}
	return iblis::CString(home, "/.local/share/meson/cross");
}

iblis::CvarStr iblis::meson::crossPath("meson_cross_path", "Path to Meson cross files directory", findMesonCrossPath());
