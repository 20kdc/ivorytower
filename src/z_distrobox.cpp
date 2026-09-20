#include "iblis.h"
#include "meson.h"
#include "names.h"

iblis::CvarBool cvar_distrobox_create("distrobox_create", "Enables/disables running 'distrobox create' to prepare containers for distrobox-based toolchains.", true);

class DistroboxComponent : public iblis::Component {
public:
	const iblis::Machine * machine;
	const char * variant;
	const iblis::CompilerCfg * compiler;
	const char * container;
	const char * image;
	DistroboxComponent(
		const char * name, const char * purpose, bool def,
		const iblis::Machine * machine, const char * variant, const iblis::CompilerCfg * compiler,
		const char * container, const char * image
	) :
		iblis::Component(name, purpose, def),
		machine(machine), variant(variant), compiler(compiler),
		container(container), image(image) {
	}
	bool install() override {
		bool looksSuccessful = true;
		// do distrobox create if necessary
		if (cvar_distrobox_create.value) {
			std::vector<std::string> dbargs;
			bool shouldCreate = true;
			/*
			dbargs.push_back("enter");
			dbargs.push_back(container);
			dbargs.push_back("--");
			dbargs.push_back("true");
			if (iblis::runCmd("distrobox", dbargs)) {
			}
			*/
			if (shouldCreate) {
				dbargs.clear();
				dbargs.push_back("create");
				dbargs.push_back("-i");
				dbargs.push_back(image);
				dbargs.push_back(container);
				if (iblis::runCmd("distrobox", dbargs))
					looksSuccessful = false;
			}
		}
		// setup
		iblis::CompilerCfg cfg = *compiler;
		auto cfgCmds = cfg.allCommands();;
		for (auto x = cfgCmds.begin(); x != cfgCmds.end(); x++) {
			std::vector<std::string> * baseCmd = *x;
			if (baseCmd->size()) {
				std::vector<std::string> newCmd = {"distrobox", "enter", container, "--"};
				for (auto y = baseCmd->begin(); y != baseCmd->end(); y++)
					newCmd.push_back(*y);
				*baseCmd = newCmd;
			}
		}
		looksSuccessful &= iblis::meson::installCrossFiles(*machine, variant, cfg);
		return looksSuccessful;
	}
};

static iblis::CompilerCfg sensibleDefaultGCC = {
	.c = {"gcc"},
	.cpp = {"g++"},
	.ar = {"ar"},
	.strip = {"strip"},
	.dispositions = {
		&iblis::STLDisposition::stl,
		&iblis::STLDisposition::staticstl,
		&iblis::STLDisposition::none,
	}
};

DistroboxComponent theSteamRTScoutComponent(
	"scout", "'scout' SteamRT Distrobox (most/all x86_64 glibc Linuxes)", true,
	&iblis::Machine::lgx64, "scout", &sensibleDefaultGCC,
	"scout", "registry.gitlab.steamos.cloud/steamrt/scout/sdk"
);
DistroboxComponent theSteamRTScouti686Component(
	"scout_i386", "'scout-i386' SteamRT Distrobox (most/all x86 glibc Linuxes)", false,
	&iblis::Machine::lgx32, "scout", &sensibleDefaultGCC,
	"scout-i386", "registry.gitlab.steamos.cloud/steamrt/scout/sdk/i386"
);
DistroboxComponent theSteamRTSniperComponent(
	"sniper", "'sniper' SteamRT Distrobox (most newer x86_64 glibc Linuxes)", false,
	&iblis::Machine::lgx64, "sniper", &sensibleDefaultGCC,
	"sniper", "registry.gitlab.steamos.cloud/steamrt/sniper/sdk"
);
