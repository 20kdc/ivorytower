#include "install.h"
#include "y_osxcross.h"
#include "names.h"

using namespace iblis;
using namespace setupcore;

class W32CrossComponent : public Component {
public:
	W32CrossComponent() : Component("w32cross", "W32Cross", true) {
	}
	bool install(InstallData * prepare) override {
		return false;
	}
};

// W32CrossComponent theW32CrossComponent;
