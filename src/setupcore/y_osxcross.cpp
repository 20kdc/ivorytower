#include "y_osxcross.h"
#include <unistd.h>

using namespace iblis;

Subsystem<OSXCrossSys> iblis::osxCrossSys;

static iblis::CvarBool cvar_osxcross_checkpath("osxcross_checkpath", "Try to find osxcross-conf via PATH before suggesting a local install.", true);

OSXCrossSys * OSXCrossSys::build() {
	HelperSys * helper = helperSys.get();
	if (!helper) {
		IBLIS_WARN("Couldn't initialize, HelperSys dead.");
		return nullptr;
	}
	if (iblis::runCmd({helper->helper("osxcross-wizard"), cvar_osxcross_checkpath.value ? "1" : "0"}) == 0) {
		auto targetinfo = readFile(helper->itsetupDir + "/osxcross.target");
		if (targetinfo.size() == 0) {
			IBLIS_WARN("OSXCross helper did not acquire OSXCROSS_TARGET");
			return nullptr;
		}
		return new OSXCrossSys(helper->osxcrossBinLink() + "/", targetinfo[0]);
	}
	return nullptr;
}

std::string OSXCrossSys::toolPath(const std::string & tool) {
	return prefix + tool;
}

std::string OSXCrossSys::archToolPath(const std::string & arch, const std::string & tool) {
	return prefix + arch + "-apple-" + target + "-" + tool;
}
