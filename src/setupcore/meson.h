#pragma once

#include "common/iblis.h"
#include "names.h"

namespace setupcore {
	struct Machine;

	namespace meson {
		extern iblis::CvarStr crossPath;
		std::string iniEscape(const std::string & src);
		std::string iniStrArray(const std::vector<std::string> & src);
		std::string iniProp(const std::string & prop, const char * src);
		// Command. Becomes a string or array based on length. If the array is empty, the command is not emitted.
		std::string iniCmd(const std::string & prop, const std::vector<std::string> & src);
		// 'Generic' property.
		// The last arg has the property appended to it.
		// Use for Meson 'internally used programs'.
		std::string iniGenCmd(const std::string & prop, const std::vector<std::string> & src);
		std::string iniArg(const std::string & prop, const std::vector<std::string> & src);
		std::string machineIni(const Machine & mach);
		std::string makeCrossFile(const CompilerCfg & comp, const STLDisposition & disposition);
		// Creates and installs cross-files.
		bool installCrossFiles(const CompilerCfg & comp);
	}
}
