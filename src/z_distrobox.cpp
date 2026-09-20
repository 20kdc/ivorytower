#include "iblis.h"
#include "meson.h"
#include "names.h"

iblis::CvarBool cvar_distrobox_create("distrobox_create", "Enables/disables running 'distrobox create' to prepare containers for distrobox-based toolchains.", true);

class DistroboxComponent : public iblis::Component {
public:
	const iblis::Machine * machine;
	const char * container;
	const char * image;
	DistroboxComponent(
		const char * name, const char * purpose, bool def,
		const iblis::Machine * machine,
		const char * container, const char * image
	) :
		iblis::Component(name, purpose, def),
		machine(machine),
		container(container), image(image) {
	}
	bool install() override {
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
				iblis::runCmd("distrobox", dbargs);
			}
		}
		// init
		return true;
	}
};

DistroboxComponent theSteamRTScoutComponent(
	"scout", "'scout' SteamRT Distrobox (most/all x86_64 glibc Linuxes)", true,
	&iblis::Machine::lgx64,
	"scout", "registry.gitlab.steamos.cloud/steamrt/scout/sdk"
);
DistroboxComponent theSteamRTScouti686Component(
	"scout_i386", "'scout-i386' SteamRT Distrobox (most/all x86 glibc Linuxes)", false,
	&iblis::Machine::lgx32,
	"scout-i386", "registry.gitlab.steamos.cloud/steamrt/scout/sdk/i386"
);
DistroboxComponent theSteamRTSniperComponent(
	"sniper", "'sniper' SteamRT Distrobox (most newer x86_64 glibc Linuxes)", false,
	&iblis::Machine::lgx64,
	"sniper", "registry.gitlab.steamos.cloud/steamrt/sniper/sdk"
);
