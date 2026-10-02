#pragma once

#include <string>
#include <vector>

namespace setupcore {
	// All as defined by Meson, except where noted.

	struct CPU {
		const char * cpu_family = 0;
		const char * cpu = 0;
		const char * endian = 0;

		CPU(const char * cpu_family, const char * cpu, const char * endian) : cpu_family(cpu_family), cpu(cpu), endian(endian) {}

		// i686 (we'll see if we need to add more of these?)
		static const CPU x32_i686;
		// x86_64
		static const CPU x64;
		// aarch64
		static const CPU a64;
	};

	struct OS {
		const char * system = 0;
		const char * subsystem = 0;
		const char * kernel = 0;

		OS(const char * system, const char * subsystem, const char * kernel) : system(system), subsystem(subsystem), kernel(kernel) {}

		static const OS lg;
		static const OS m;
		static const OS w;
	};

	struct Machine {
		// Custom
		const char * ivtName = 0;

		const OS * os;
		const CPU * cpu;

		Machine(const char * ivtName, const OS * os, const CPU * cpu) : ivtName(ivtName), os(os), cpu(cpu) {}

		static const Machine lgx32;
		static const Machine lgx64;
		static const Machine lga64;

		static const Machine mx32;
		static const Machine mx64;
		static const Machine ma64;

		static const Machine wx32;
		static const Machine wx64;
		static const Machine wa64;
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
		const char * bVSCRT = 0;
		void merge(const CompilerArgs & other);
	};

	struct STLDisposition {
		const char * ivtName = nullptr;
		// Compile C++ with the C compiler. This is used to force q_zero to work on older GCC.
		bool hackCPPWithC = false;
		CompilerArgs compilerArgs;

		static const STLDisposition q_default;
		static const STLDisposition q_static;
		static const STLDisposition q_staticW;
		static const STLDisposition q_zero;
		static const STLDisposition q_zeroW;
		static const STLDisposition q_zeroL;
	};

	struct CompilerCfg {
		// Machine this compiler config is for.
		const Machine * machine;

		// Variant (i.e. "scout", "sniper")
		std::string variant;

		// 'Core' compilation tools.
		std::vector<std::string> c;
		std::vector<std::string> c_ld;
		std::vector<std::string> cpp;
		std::vector<std::string> cpp_ld;
		std::vector<std::string> ar;
		std::vector<std::string> windres;
		std::vector<std::string> lib;
		std::vector<std::string> strip;

		// CMake. Would be in generic but mingw etc. don't work this way, so we need to not prefix there.
		std::vector<std::string> cmake;

		// Generic prefix.
		// This controls all *-config series programs.
		std::vector<std::string> generic;

		// STL dispositions.
		std::vector<const STLDisposition *> dispositions;

		// Compiler args.
		CompilerArgs args;
		std::vector<std::vector<std::string> *> allCommands();
	};
}
