#pragma once

#include <string>
#include <vector>

namespace iblis {
	struct Machine {
		const char * ivtName = 0;

		// All as defined by Meson:
		const char * system = 0;
		const char * subsystem = 0;
		const char * kernel = 0;
		const char * cpu_family = 0;
		const char * cpu = 0;
		const char * endian = 0;

		static const Machine lgx32;
		static const Machine lgx64;
		static const Machine ma64;
		static const Machine mx64;
		static const Machine wx32;
		static const Machine wx64;
	};

	// Common config between STLDisposition and CompilerCfg
	struct CompilerArgs {
		std::vector<std::string> addCArgs;
		std::vector<std::string> addCLinkArgs;
		std::vector<std::string> addCppArgs;
		std::vector<std::string> addCppLinkArgs;
		// if empty these are left alone
		const char * cppEh = 0;
		const char * cppRtti = 0;
		void merge(const CompilerArgs & other);
	};

	struct STLDisposition {
		const char * ivtName;
		CompilerArgs compilerArgs;

		static const STLDisposition stl;
		static const STLDisposition staticstl;
		static const STLDisposition none;
	};

	struct CompilerCfg {
		std::vector<std::string> c;
		std::vector<std::string> cpp;
		std::vector<std::string> ar;
		std::vector<std::string> windres;
		std::vector<std::string> strip;
		std::vector<const STLDisposition *> dispositions;
		CompilerArgs args;
		std::vector<std::vector<std::string> *> allCommands();
	};
}
