#include "install.h"
#include "y_osxcross.h"
#include "names.h"
#include "meson.h"

class OSXCrossComponent : public iblis::Component {
public:
	OSXCrossComponent() : iblis::Component("osxcross", "OSXCross", true) {
	}
	bool subinstall(iblis::OSXCrossSys * osxcross, const iblis::Machine & mach, const std::string & archpfx) {
		iblis::CompilerCfg compiler;
		compiler.c = {osxcross->toolPath(archpfx + "-clang")},
		compiler.cpp = {osxcross->toolPath(archpfx + "-clang++")},
		// We know through testing that for cctools stable:
		// 'ar' and 'strip' only have one implementation, not per-architecture
		compiler.ar = {osxcross->toolPath("xcrun"), "ar"},
		compiler.strip = {osxcross->toolPath("xcrun"), "strip"},
		compiler.dispositions = {
			&iblis::STLDisposition::stl,
			&iblis::STLDisposition::staticstl,
			&iblis::STLDisposition::none,
		};
		return iblis::meson::installCrossFiles(iblis::Machine::mx32, "", compiler);
	}
	bool install() override {
		iblis::OSXCrossSys * osxcross = iblis::osxCrossSys.get();
		if (!osxcross) {
			IBLIS_WARN("Missing OSXCross.");
			return false;
		}
		// Ok, so, here's the deal.
		// OSXCross has a few different binary arrangements.
		// My favorite is the `o64`/`oa64` series.
		bool looksSuccessful = true;
		looksSuccessful &= subinstall(osxcross, iblis::Machine::mx32, "o32");
		looksSuccessful &= subinstall(osxcross, iblis::Machine::mx64, "o64");
		looksSuccessful &= subinstall(osxcross, iblis::Machine::ma64, "oa64");
		return looksSuccessful;
	}
};

OSXCrossComponent theOSXCrossComponent;
