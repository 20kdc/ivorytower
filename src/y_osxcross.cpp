#include "y_osxcross.h"

using namespace iblis;

Subsystem<OSXCrossSys> osxCrossSys;

OSXCrossSys * OSXCrossSys::build() {
	IBLIS_WARN("Not implemented.");
	return nullptr;
}

std::string OSXCrossSys::toolPath(const std::string & tool) {
	return prefix + tool;
}

