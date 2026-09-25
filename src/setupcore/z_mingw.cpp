#include "install.h"
#include "meson.h"
#include "names.h"

using namespace iblis;
using namespace setupcore;

CvarStr cvar_mingw_dist("mingw_dist", "MinGW distribution indicator.", "-w64-mingw32-");
CvarStr cvar_mingw_suffix("mingw_suffix", "MinGW compiler suffix. Used to add '-win32'.", "-win32");

class MinGWComponent : public Component {
public:
	MinGWComponent() : Component("mingw", "MinGW-w64 setup", true) {
	}
	std::string formatCompCom(const char * arch, const char * original, int suffix) {
		std::string res;
		res += arch;
		res += cvar_mingw_dist.value;
		res += original;
		if (suffix)
			res += cvar_mingw_suffix.value;
		return res;
	}
	bool subinstall(InstallData * prepare, const Machine & mach, const char * archpfx) {
		CompilerCfg compiler;
		compiler.machine = &mach,
		compiler.variant = "",
		compiler.c = {formatCompCom(archpfx, "gcc", 1)},
		compiler.cpp = {formatCompCom(archpfx, "g++", 1)},
		compiler.ar = {formatCompCom(archpfx, "ar", 0)},
		compiler.windres = {formatCompCom(archpfx, "windres", 0)},
		compiler.strip = {formatCompCom(archpfx, "strip", 0)},
		compiler.dispositions = {
			&STLDisposition::q_default,
			&STLDisposition::q_staticW,
			&STLDisposition::q_zeroW,
		};
		prepare->addTarget(compiler);
		return true;
	}
	bool install(InstallData * prepare) override {
		bool looksSuccessful = true;
		looksSuccessful &= subinstall(prepare, Machine::wx32, "i686");
		looksSuccessful &= subinstall(prepare, Machine::wx64, "x86_64");
		looksSuccessful &= subinstall(prepare, Machine::wa64, "aarch64");
		return looksSuccessful;
	}
};

MinGWComponent theMinGWComponent;
