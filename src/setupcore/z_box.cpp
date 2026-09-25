#include "install.h"
#include "meson.h"
#include "names.h"
#include "y_boxenrunner.h"

using namespace iblis;
using namespace setupcore;

class ContainerCCComponent : public Component {
public:
	const Machine * machine;
	const char * variant;
	const CompilerCfg * compiler;
	const char * container;
	const char * image;
	ContainerCCComponent(
		const char * name, const char * purpose, bool def,
		const Machine * machine, const char * variant, const CompilerCfg * compiler,
		const char * container, const char * image
	) :
		Component(name, purpose, def),
		machine(machine), variant(variant), compiler(compiler),
		container(container), image(image) {
	}
	bool install(InstallData * prepare) override {
		auto boxSys = iblis::boxenrunnerSys.get();
		if (!boxSys) {
			IBLIS_WARN("BoxenrunnerSys dead");
			return false;
		}
		bool looksSuccessful = true;
		// do distrobox create if necessary
		// we skip the test entirely if we won't create it anyways
		if (iblis::cvar_box_create.value) {
			bool exists = boxSys->test(container);
			if (!exists)
				if (!boxSys->create(container, image))
					looksSuccessful = false;
		}
		// setup
		setupcore::CompilerCfg cfg = *compiler;
		cfg.machine = machine;
		cfg.variant = variant;
		auto cfgCmds = cfg.allCommands();
		for (auto x = cfgCmds.begin(); x != cfgCmds.end(); x++) {
			std::vector<std::string> * baseCmd = *x;
			if (baseCmd->size()) {
				std::vector<std::string> newCmd = boxSys->prefix(container);
				for (auto y = baseCmd->begin(); y != baseCmd->end(); y++)
					newCmd.push_back(*y);
				*baseCmd = newCmd;
			}
		}
		prepare->addTarget(cfg);
		return looksSuccessful;
	}
};

static CompilerCfg sensibleDefaultGCC = [] {
	CompilerCfg result;
	result.c = {"gcc"};
	result.cpp = {"g++"};
	result.ar = {"ar"};
	result.strip = {"strip"};
	result.dispositions = {
		&STLDisposition::q_default,
		&STLDisposition::q_static,
		&STLDisposition::q_zeroL,
	};
	return result;
}();

ContainerCCComponent theSteamRTScoutComponent(
	"scout", "'scout' SteamRT (most/all x86_64 glibc Linuxes)", true,
	&Machine::lgx64, "scout", &sensibleDefaultGCC,
	"scout", "registry.gitlab.steamos.cloud/steamrt/scout/sdk"
);
ContainerCCComponent theSteamRTScouti686Component(
	"scout_i386", "'scout-i386' SteamRT (most/all x86 glibc Linuxes)", false,
	&Machine::lgx32, "scout", &sensibleDefaultGCC,
	"scout-i386", "registry.gitlab.steamos.cloud/steamrt/scout/sdk/i386"
);
ContainerCCComponent theSteamRTSniperComponent(
	"sniper", "'sniper' SteamRT (most newer x86_64 glibc Linuxes)", false,
	&Machine::lgx64, "sniper", &sensibleDefaultGCC,
	"sniper", "registry.gitlab.steamos.cloud/steamrt/sniper/sdk"
);
