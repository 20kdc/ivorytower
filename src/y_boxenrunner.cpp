#include "y_boxenrunner.h"
#include "src/iblis.h"

using namespace iblis;

static CvarStr cvar_boxenrunner("box", "The 'box wrapper' used for crosscompilation.", "distrobox");

BoxenrunnerSys * iblis::BoxenrunnerSys::build() {
	auto helpy = helperSys.get();
	if (!helpy) {
		IBLIS_WARN("Couldn't initialize, HelperSys dead.");
		return nullptr;
	}
	std::string helper = helpy->helper("boxenrunner");
	std::string runner = cvar_boxenrunner.value;
	if (iblis::runCmd({helper, runner, "check"})) {
		IBLIS_WARN("Couldn't initialize, selftest failed.");
		return nullptr;
	}
	return new iblis::BoxenrunnerSys(helper, runner);
}

bool iblis::BoxenrunnerSys::create(const std::string & container, const std::string & image) {
	return iblis::runCmd({helper, runner, "create", container, image}) == 0;
}

bool iblis::BoxenrunnerSys::test(const std::string & container) {
	return iblis::runCmd({helper, runner, "test", container}) == 0;
}

std::vector<std::string> iblis::BoxenrunnerSys::prefix(const std::string & container) {
	if (runner == "distrobox") {
		// decouple generated files from boxenrunner if on distrobox
		return {"distrobox", "enter", container, "--"};
	} else {
		return {helper, runner, "enter", container};
	}
}

Subsystem<BoxenrunnerSys> iblis::boxenrunnerSys;
