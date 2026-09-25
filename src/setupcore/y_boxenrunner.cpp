#include "y_boxenrunner.h"

using namespace iblis;

static CvarStr cvar_boxenrunner("box",
	"The 'box wrapper' used for crosscompilation.\n"
	"  ivt_docker: uses Docker to manage unprivileged containers.\n"
	"  distrobox: one of the steamrt recommended boxes, but runs --privileged. Recommend export DBX_CONTAINER_MANAGER=docker in profile.\n"
	"  ", "ivt_docker");
static iblis::CvarStr cvar_box_prefix("box_prefix", "Prefix for containers created/used by ivorytower.", "");
iblis::CvarBool iblis::cvar_box_create("box_create", "Control creating containers. If 0, operations which create containers will proceed as if they were created if possible.", true);

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
	if (!cvar_box_create.value)
		return true;
	std::string containerRes = cvar_box_prefix.value + container;
	return iblis::runCmd({helper, runner, "create", containerRes, image}) == 0;
}

bool iblis::BoxenrunnerSys::test(const std::string & container) {
	std::string containerRes = cvar_box_prefix.value + container;
	return iblis::runCmd({helper, runner, "test", containerRes}) == 0;
}

std::vector<std::string> iblis::BoxenrunnerSys::prefix(const std::string & container) {
	std::string containerRes = cvar_box_prefix.value + container;
	if (runner == "distrobox") {
		// decouple generated files from boxenrunner if on distrobox
		// we can't do this for ivt_docker, since boxenrunner manages that.
		return {"distrobox", "enter", containerRes, "--"};
	} else {
		return {helper, runner, "enter", containerRes};
	}
}

Subsystem<BoxenrunnerSys> iblis::boxenrunnerSys;
