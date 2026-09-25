#include "install.h"
#include "y_osxcross.h"
#include "names.h"

using namespace iblis;
using namespace setupcore;

class OSXCrossComponent : public Component {
public:
	OSXCrossComponent() : Component("osxcross", "OSXCross", true) {
	}
	bool subinstall(InstallData * prepare, iblis::OSXCrossSys * osxcross, const Machine & mach, const std::string & archpfx) {
		CompilerCfg compiler;
		compiler.machine = &mach;
		compiler.variant = "";
		compiler.c = {osxcross->archToolPath(archpfx, "clang")},
		compiler.cpp = {osxcross->archToolPath(archpfx, "clang++")},
		compiler.ar = {osxcross->archToolPath(archpfx, "ar")},
		compiler.strip = {osxcross->archToolPath(archpfx, "strip")},
		compiler.generic = {osxcross->archToolPath(archpfx, "")};
		compiler.dispositions = {
			&STLDisposition::q_default,
			&STLDisposition::q_static,
			&STLDisposition::q_zero,
		};
		prepare->addTarget(compiler);
		return true;
	}
	bool install(InstallData * prepare) override {
		iblis::OSXCrossSys * osxcross = iblis::osxCrossSys.get();
		if (!osxcross) {
			IBLIS_WARN("Missing OSXCross.");
			return false;
		}
		// Ok, so, here's the deal.
		// OSXCross has a few different binary arrangements.
		// My favorite is the `o64`/`oa64` series.
		bool looksSuccessful = true;
		// Ignore the CPU disaprity here; see Machine constant for rationale.
		looksSuccessful &= subinstall(prepare, osxcross, Machine::mx32, "i386");
		looksSuccessful &= subinstall(prepare, osxcross, Machine::mx64, "x86_64");
		// important and scary: osxcross canonically calls this arm64
		// it has some aarch64 links - but they don't work if you want CMake!
		looksSuccessful &= subinstall(prepare, osxcross, Machine::ma64, "arm64");
		return looksSuccessful;
	}
};

OSXCrossComponent theOSXCrossComponent;
