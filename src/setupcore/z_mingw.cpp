#include "install.h"
#include "meson.h"
#include "names.h"

iblis::CvarStr cvar_mingw_dist("mingw_dist", "MinGW distribution indicator.", "-w64-mingw32-");
iblis::CvarStr cvar_mingw_suffix("mingw_suffix", "MinGW compiler suffix. Used to add '-win32'.", "-win32");

class MinGWComponent : public iblis::Component {
public:
	MinGWComponent() : iblis::Component("mingw", "MinGW-w64 setup", true) {
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
	bool install() override {
		iblis::CompilerCfg compilerWX32 = {
			.c = {formatCompCom("i686", "gcc", 1)},
			.cpp = {formatCompCom("i686", "g++", 1)},
			.ar = {formatCompCom("i686", "ar", 0)},
			.windres = {formatCompCom("i686", "windres", 0)},
			.strip = {formatCompCom("i686", "strip", 0)},
			.dispositions = {
				&iblis::STLDisposition::stl,
				&iblis::STLDisposition::staticstl,
				&iblis::STLDisposition::none,
			}
		};
		iblis::CompilerCfg compilerWX64 = {
			.c = {formatCompCom("x86_64", "gcc", 1)},
			.cpp = {formatCompCom("x86_64", "g++", 1)},
			.ar = {formatCompCom("x86_64", "ar", 0)},
			.windres = {formatCompCom("x86_64", "windres", 0)},
			.strip = {formatCompCom("x86_64", "strip", 0)},
			.dispositions = {
				&iblis::STLDisposition::stl,
				&iblis::STLDisposition::staticstl,
				&iblis::STLDisposition::none,
			}
		};
		bool looksSuccessful = true;
		looksSuccessful &= iblis::meson::installCrossFiles(iblis::Machine::wx32, "", compilerWX32);
		looksSuccessful &= iblis::meson::installCrossFiles(iblis::Machine::wx64, "", compilerWX64);
		return looksSuccessful;
	}
};

MinGWComponent theMinGWComponent;
