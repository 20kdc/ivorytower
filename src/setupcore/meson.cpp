#include <stdlib.h>
#include <stdio.h>

#include "common/iblis.h"
#include "names.h"
#include "meson.h"

using namespace iblis;
using namespace setupcore;

std::string findMesonCrossPath() {
	const char * home = getenv("HOME");
	if (!home) {
		IBLIS_WARN("missing HOME environment variable");
		exit(1);
	}
	return std::string(home) + "/.local/share/meson/cross/ivt";
}

CvarStr setupcore::meson::crossPath("meson_cross_path", "Path to Meson cross files directory", findMesonCrossPath());

std::string setupcore::meson::iniEscape(const std::string & src) {
	std::string res;
	res += "'";
	res += src;
	res += "'";
	return res;
}

std::string setupcore::meson::iniStrArray(const std::vector<std::string> & src) {
	std::string res;
	res += "[";
	for (auto i = src.begin(); i != src.end(); i++) {
		if (i != src.begin())
			res += ", ";
		res += iniEscape(*i);
	}
	res += "]";
	return res;
}

std::string setupcore::meson::iniProp(const std::string & prop, const char * src) {
	std::string res;
	if (!src)
		return res;
	res += prop;
	res += " = ";
	res += iniEscape(src);
	res += "\n";
	return res;
}

std::string setupcore::meson::iniCmd(const std::string & prop, const std::vector<std::string> & src) {
	std::string res;
	if (src.size() == 0)
		return res;
	res += prop;
	res += " = ";
	if (src.size() == 1) {
		res += iniEscape(src[0]);
	} else {
		res += iniStrArray(src);
	}
	res += "\n";
	return res;
}

std::string setupcore::meson::iniGenCmd(const std::string & prop, const std::vector<std::string> & src) {
	std::string res;
	if (src.size() == 0)
		return res;
	std::vector<std::string> dst = src;
	auto & last = dst[dst.size() - 1];
	last += prop;
	res += prop;
	res += " = ";
	if (dst.size() == 1) {
		res += iniEscape(dst[0]);
	} else {
		res += iniStrArray(dst);
	}
	res += "\n";
	return res;
}

std::string setupcore::meson::iniArg(const std::string & prop, const std::vector<std::string> & src) {
	std::string res;
	if (src.size() == 0)
		return res;
	res += prop;
	res += " = ";
	res += iniStrArray(src);
	res += "\n";
	return res;
}

std::string setupcore::meson::machineIni(const Machine & mach) {
	std::string base;
	base += iniProp("system", mach.os->system);
	base += iniProp("subsystem", mach.os->subsystem);
	base += iniProp("kernel", mach.os->kernel);
	base += iniProp("cpu_family", mach.cpu->cpu_family);
	base += iniProp("cpu", mach.cpu->cpu);
	base += iniProp("endian", mach.cpu->endian);
	return base;
}

std::string setupcore::meson::makeCrossFile(const CompilerCfg & comp, const STLDisposition & disposition) {
	std::string base;
	base += "[host_machine]\n";
	base += machineIni(*comp.machine);
	base += "[properties]\n";
	// By doing this and not setting an EXE wrapper, we disable Meson's sanity checker.
	// This matters a LOT for cases like mingw dynamic STL, which we'd need to somehow locate and then sneakily install DLL copy commands into Meson to fix.
	// By the way, the libraries now hang out in crazy places like `/usr/lib/gcc/i686-w64-mingw32/13-win32/libstdc++-6.dll`.
	// ...For the record, my bet on the best 'fix' for this is to use `gcc --version -v` to extract the `LIBRARY_PATH` line.
	// The problem is that Meson doesn't have a good way to actually pull the libraries (if it did, I suspect it'd probably do this itself).
	// The execution part of the sanity check tests the *wrapper* more than it does the *compiler*.
	// We might be able to get a little more fine-grained later, but for now it's good enough to just sabotage it.
	base += "needs_exe_wrapper = true\n";
	base += "[binaries]\n";
	base += iniCmd("c", comp.c);
	base += iniCmd("c_ld", comp.c_ld);
	if (disposition.hackCPPWithC) {
		// SteamRT Scout needs this as a workaround for no -nostdlib++
		base += iniCmd("cpp", comp.c);
		base += iniCmd("cpp_ld", comp.c_ld);
	} else {
		base += iniCmd("cpp", comp.cpp);
		base += iniCmd("cpp_ld", comp.cpp_ld);
	}
	// Very nasty hacky code for Mac reasons.
	// Both GCC and Clang consider Objective-C/C++ as a C/C++ extension.
	base += iniCmd("objc", comp.c);
	base += iniCmd("objc_ld", comp.c_ld);
	base += iniCmd("objcpp", comp.cpp);
	base += iniCmd("objcpp_ld", comp.cpp_ld);

	base += iniCmd("ar", comp.ar);
	base += iniCmd("windres", comp.windres);
	base += iniCmd("strip", comp.strip);
	base += iniCmd("cmake", comp.cmake);

	// https://mesonbuild.com/Machine-files.html#binaries
	base += iniGenCmd("cups-config", comp.generic);
	base += iniGenCmd("gnustep-config", comp.generic);
	base += iniGenCmd("gpgme-config", comp.generic);
	base += iniGenCmd("libgcrypt-config", comp.generic);
	base += iniGenCmd("libwmf-config", comp.generic);
	base += iniGenCmd("llvm-config", comp.generic);
	base += iniGenCmd("pcap-config", comp.generic);
	base += iniGenCmd("pkg-config", comp.generic);
	base += iniGenCmd("sdl2-config", comp.generic);
	base += iniGenCmd("wx-config", comp.generic);
	base += iniGenCmd("wx-3.0-config", comp.generic);
	base += iniGenCmd("wx-config-gtk", comp.generic);

	base += "[built-in options]\n";
	CompilerArgs mergedArgs = comp.args;
	mergedArgs.merge(disposition.compilerArgs);
	base += iniArg("c_args", mergedArgs.addCArgs);
	base += iniArg("c_link_args", mergedArgs.addCLinkArgs);
	base += iniArg("cpp_args", mergedArgs.addCppArgs);
	base += iniArg("cpp_link_args", mergedArgs.addCppLinkArgs);
	// Continued Mac nonsense.
	base += iniArg("objc_args", mergedArgs.addCArgs);
	base += iniArg("objc_link_args", mergedArgs.addCLinkArgs);
	base += iniArg("objcpp_args", mergedArgs.addCppArgs);
	base += iniArg("objcpp_link_args", mergedArgs.addCppLinkArgs);

	base += iniProp("cpp_eh", mergedArgs.cppEh);
	base += iniProp("cpp_rtti", mergedArgs.cppRtti);
	base += iniProp("b_vscrt", mergedArgs.bVSCRT);
	return base;
}

static bool hasDoneCrossFileMkdir = false;

bool setupcore::meson::installCrossFiles(const CompilerCfg & comp) {
	bool allOk = true;
	for (auto disposition = comp.dispositions.begin(); disposition != comp.dispositions.end(); disposition++) {
		auto iniContent = makeCrossFile(comp, **disposition);
		std::string referent = "";
		referent += comp.machine->ivtName;
		referent += "_";
		referent += (*disposition)->ivtName;
		if (comp.variant.length() > 0) {
			referent += "_";
			referent += comp.variant;
		}
		auto path = crossPath.value;
		path += "/";
		path += referent;
		if (!hasDoneCrossFileMkdir) {
			hasDoneCrossFileMkdir = true;
			runCmd({"mkdir", "-p", crossPath.value});
		}
		printf(" Installing Meson crossfile 'ivt/%s'.\n", referent.c_str());
		bool res = writeFile(path, iniContent);
		if (!res) {
			printf("  ...failed\n");
			allOk = false;
		}
	}
	return allOk;
}
