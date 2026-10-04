#include "install.h"
#include "y_w32cross.h"
#include "names.h"

using namespace iblis;
using namespace setupcore;

class W32CrossComponent : public Component {
public:
	Subsystem<W32CrossSys> * sdk;
	W32CrossComponent(const char * name, const char * purpose, Subsystem<W32CrossSys> * sdk) : Component(name, purpose, true), sdk(sdk) {
	}
	bool subinstall(InstallData * prepare, iblis::W32CrossSys * sdkP, const Machine & mach, const std::string & arch) {
		CompilerCfg compiler;
		compiler.machine = &mach;
		compiler.variant = "w32cross";
		// To make clangd work properly, this hell had to be created.
		compiler.c = {"clang"};
		compiler.cpp = {"clang++"};

		compiler.args.addCArgs = sdkP->getClangArgs(arch);
		compiler.args.addCLinkArgs = sdkP->getClangArgs(arch + ".ld");

		compiler.args.addCppArgs = compiler.args.addCArgs;
		compiler.args.addCppLinkArgs = compiler.args.addCLinkArgs;

		compiler.args.bVSCRT = "md";

		compiler.ar = {sdkP->toolCompatPath("llvm-lib")},
		compiler.strip = {sdkP->toolPath("w32cross-strip")};
		compiler.windres = {sdkP->toolPath("w32cross-windres-" + arch)};
		//compiler.generic = {sdkP->toolPath("")};
		compiler.dispositions = {
			// We only support 'default' STL disposition for this compiler.
			// Explanation is in w32cross/doc/WHY_STL.md
			&STLDisposition::q_default,
		};
		prepare->addTarget(compiler);
		return true;
	}
	bool install(InstallData * prepare) override {
		iblis::W32CrossSys * sdkP = sdk->get();
		if (!sdkP) {
			IBLIS_WARN("Missing W32Cross.");
			return false;
		}
		bool looksSuccessful = true;
		looksSuccessful &= subinstall(prepare, sdkP, Machine::wx32, "x86");
		looksSuccessful &= subinstall(prepare, sdkP, Machine::wx64, "x64");
		looksSuccessful &= subinstall(prepare, sdkP, Machine::wa64, "arm64");
		return looksSuccessful;
	}
};

W32CrossComponent theW32CrossW10Component("w32cross_w10", "W32Cross 'w10' SDK", &w32CrossSys_w10);
