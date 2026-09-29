#include "install.h"
#include "y_osxcross.h"
#include "names.h"

using namespace iblis;
using namespace setupcore;

class W32CrossComponent : public Component {
public:
	const char * sdk;
	W32CrossComponent(const char * name, const char * purpose, const char * sdk) : Component(name, purpose, true), sdk(sdk) {
	}
	bool install(InstallData * prepare) override {
		return false;
	}
};

W32CrossComponent theW32CrossComponent("w32cross_w10", "W32Cross 'w10' SDK", "w10");
