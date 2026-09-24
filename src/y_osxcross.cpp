#include "y_osxcross.h"

using namespace iblis;

Subsystem<OSXCrossSys> iblis::osxCrossSys;

static iblis::CvarBool cvar_osxcross_checkpath("osxcross_checkpath", "Try to find osxcross-conf via PATH before suggesting a local install.", true);

OSXCrossSys * OSXCrossSys::build() {
	HelperSys * helper = helperSys.get();
	if (!helper) {
		IBLIS_WARN("Couldn't initialize, HelperSys dead.");
		return nullptr;
	}
	if (iblis::runCmd({helper->helper("osxcross-wizard"), cvar_osxcross_checkpath.value ? "1" : "0"}) == 0)
		return new OSXCrossSys(helper->osxcrossBinLink() + "/");
	return nullptr;
}

std::string OSXCrossSys::toolPath(const std::string & tool) {
	return prefix + tool;
}
