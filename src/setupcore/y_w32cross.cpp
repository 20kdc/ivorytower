#include "y_w32cross.h"
#include <unistd.h>

using namespace iblis;

Subsystem<W32CrossSys> iblis::w32CrossSys;

W32CrossSys * W32CrossSys::build() {
	HelperSys * helper = helperSys.get();
	if (!helper) {
		IBLIS_WARN("Couldn't initialize, HelperSys dead.");
		return nullptr;
	}
	if (iblis::runCmd({helper->helper("w32cross-wizard")}) == 0) {
		return new W32CrossSys(helper->w32crossBinLink() + "/");
	}
	return nullptr;
}

std::string W32CrossSys::toolPath(const std::string & tool) {
	return prefix + tool;
}
