#pragma once

#include "common/iblis.h"
#include "names.h"

namespace iblis {
	struct Machine;

	namespace meson {
		extern iblis::CvarStr crossPath;
		std::string iniEscape(const std::string & src);
		std::string iniStrArray(const std::vector<std::string> & src);
		std::string iniProp(const std::string & prop, const char * src);
		std::string iniCmd(const std::string & prop, const std::vector<std::string> & src);
		std::string iniArg(const std::string & prop, const std::vector<std::string> & src);
		std::string machineIni(const iblis::Machine & mach);
		std::string makeCrossFile(const iblis::Machine & mach, const iblis::STLDisposition & disposition, const iblis::CompilerCfg & comp);
		// Creates and installs cross-files.
		bool installCrossFiles(const iblis::Machine & mach, const std::string & variant, const iblis::CompilerCfg & comp);
	}
}
