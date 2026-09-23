#include "iblis.h"

class OSXCrossComponent : public iblis::Component {
public:
	OSXCrossComponent() : iblis::Component("osxcross", "OSXCross (NYI)", true) {
	}
	std::string formatCompCom(const char * arch, const char * original, int suffix) {
		std::string res;
		res += arch;
		res += original;
		return res;
	}
	bool install() override {
		IBLIS_WARN("Not yet implemented. Continuing as if succeeded.");
		return true;
	}
};

OSXCrossComponent theOSXCrossComponent;
