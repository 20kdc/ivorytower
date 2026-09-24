#include "iblis.h"
#include "y_osxcross.h"
#include "names.h"
#include "meson.h"

class OSXCrossComponent : public iblis::Component {
public:
	OSXCrossComponent() : iblis::Component("osxcross", "OSXCross", true) {
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
		iblis::CompilerCfg compilerMX32 = {
			.c = {osxcross->toolPath("o32-clang")},
			.cpp = {osxcross->toolPath("o32-clang++")},
			// We know through testing that for cctools stable:
			// 'ar' and 'strip' only have one implementation, not per-architecture
			.ar = {osxcross->toolPath("xcrun"), "ar"},
			.strip = {osxcross->toolPath("xcrun"), "strip"},
			.dispositions = {
				&iblis::STLDisposition::stl,
				&iblis::STLDisposition::staticstl,
				&iblis::STLDisposition::none,
			}
		};
		iblis::CompilerCfg compilerMX64 = {
			.c = {osxcross->toolPath("o64-clang")},
			.cpp = {osxcross->toolPath("o64-clang++")},
			// We know through testing that for cctools stable:
			// 'ar' and 'strip' only have one implementation, not per-architecture
			.ar = {osxcross->toolPath("xcrun"), "ar"},
			.strip = {osxcross->toolPath("xcrun"), "strip"},
			.dispositions = {
				&iblis::STLDisposition::stl,
				&iblis::STLDisposition::staticstl,
				&iblis::STLDisposition::none,
			}
		};
		iblis::CompilerCfg compilerMA64 = {
			.c = {osxcross->toolPath("oa64-clang")},
			.cpp = {osxcross->toolPath("oa64-clang++")},
			// We know through testing that for cctools stable:
			// 'ar' and 'strip' only have one implementation, not per-architecture
			.ar = {osxcross->toolPath("xcrun"), "ar"},
			.strip = {osxcross->toolPath("xcrun"), "strip"},
			.dispositions = {
				&iblis::STLDisposition::stl,
				&iblis::STLDisposition::staticstl,
				&iblis::STLDisposition::none,
			}
		};
		bool looksSuccessful = true;
		looksSuccessful &= iblis::meson::installCrossFiles(iblis::Machine::mx32, "", compilerMX32);
		looksSuccessful &= iblis::meson::installCrossFiles(iblis::Machine::mx64, "", compilerMX64);
		looksSuccessful &= iblis::meson::installCrossFiles(iblis::Machine::ma64, "", compilerMA64);
		return looksSuccessful;
	}
};

OSXCrossComponent theOSXCrossComponent;
