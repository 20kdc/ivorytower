#include "y_w32cross.h"
#include <unistd.h>

using namespace iblis;

Subsystem<W32CrossSys> iblis::w32CrossSys_w10("w10");

W32CrossSys * W32CrossSys::build(const char * sdk) {
	HelperSys * helper = helperSys.get();
	if (!helper) {
		IBLIS_WARN("Couldn't initialize, HelperSys dead.");
		return nullptr;
	}
	if (iblis::runCmd({helper->helper("w32cross-wizard"), sdk}) == 0) {
		return new W32CrossSys(helper->w32crossSDK(sdk) + "/");
	}
	return nullptr;
}
