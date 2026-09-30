#include "names.h"

using namespace setupcore;

/* CPU defintiions */

const CPU CPU::x32_i686("x86", "i686", "little");
const CPU CPU::x64("x86_64", "x86_64", "little");
const CPU CPU::a64("aarch64", "aarch64", "little");

/* OS defintiions */

const OS OS::lg("linux", nullptr, "linux");
const OS OS::m("darwin", "macos", "xnu");
const OS OS::w("windows", "windows", "nt");

/* machine definitions */

const Machine Machine::lgx32("lgx32", &OS::lg, &CPU::x32_i686);
const Machine Machine::lgx64("lgx64", &OS::lg, &CPU::x64);
const Machine Machine::lga64("lga64", &OS::lg, &CPU::a64);

// While the written name is i386, this seems more due to tooling conventions.
// Intel Macs started with Core Duo. Assume i686.
const Machine Machine::mx32("mx32", &OS::m, &CPU::x32_i686);
const Machine Machine::mx64("mx64", &OS::m, &CPU::x64);
const Machine Machine::ma64("ma64", &OS::m, &CPU::a64);

const Machine Machine::wx32("wx32", &OS::w, &CPU::x32_i686);
const Machine Machine::wx64("wx64", &OS::w, &CPU::x64);
const Machine Machine::wa64("wa64", &OS::w, &CPU::a64);

const STLDisposition STLDisposition::q_default = [] {
	STLDisposition result;
	result.ivtName = "default";
	return result;
}();

const STLDisposition STLDisposition::q_static = [] {
	STLDisposition result;
	result.ivtName = "static";
	result.compilerArgs.addCppLinkArgs = {"-static-libstdc++"};
	return result;
}();

const STLDisposition STLDisposition::q_zero = [] {
	STLDisposition result;
	result.ivtName = "zero";
	result.compilerArgs.addCArgs = {"-fno-exceptions"};
	result.compilerArgs.addCppArgs = {"-nostdinc++", "-fno-rtti", "-fno-exceptions"};
	result.compilerArgs.addCppLinkArgs = {"-nostdlib++"};
	result.compilerArgs.cppEh = "none";
	result.compilerArgs.cppRtti = "false";
	return result;
}();

// Recommend looking at /usr/lib/gcc/x86_64-w64-mingw32/13-win32/ for reference here.

const STLDisposition STLDisposition::q_staticW = [] {
	STLDisposition result;
	result.ivtName = "static",
	result.compilerArgs.addCLinkArgs = {"-static-libgcc"};
	result.compilerArgs.addCppLinkArgs = {"-static-libstdc++", "-static-libgcc", "-l:libatomic.a"};
	return result;
}();

const STLDisposition STLDisposition::q_zeroW = [] {
	STLDisposition result;
	result.ivtName = "zero";
	result.compilerArgs.addCArgs = {"-fno-exceptions"};
	result.compilerArgs.addCLinkArgs = {"-static-libgcc"};
	result.compilerArgs.addCppArgs = {"-nostdinc++", "-fno-rtti", "-fno-exceptions"};
	result.compilerArgs.addCppLinkArgs = {"-nostdlib++", "-static-libgcc", "-l:libatomic.a"};
	result.compilerArgs.cppEh = "none";
	result.compilerArgs.cppRtti = "false";
	return result;
}();

// Old Linux means old GCC. This requires we use hackCPPWithC, which was how you did this before -nostdlib++.

const STLDisposition STLDisposition::q_zeroL = [] {
	STLDisposition result;
	result.ivtName = "zero";
	result.hackCPPWithC = true;
	result.compilerArgs.addCArgs = {"-fno-exceptions"};
	result.compilerArgs.addCppArgs = {"-fno-rtti", "-fno-exceptions"};
	result.compilerArgs.cppEh = "none";
	result.compilerArgs.cppRtti = "false";
	return result;
}();

// Utilities

void CompilerArgs::merge(const CompilerArgs & other) {
	for (auto x = other.addCArgs.begin(); x < other.addCArgs.end(); x++)
		addCArgs.push_back(*x);
	for (auto x = other.addCLinkArgs.begin(); x < other.addCLinkArgs.end(); x++)
		addCLinkArgs.push_back(*x);
	for (auto x = other.addCppArgs.begin(); x < other.addCppArgs.end(); x++)
		addCppArgs.push_back(*x);
	for (auto x = other.addCppLinkArgs.begin(); x < other.addCppLinkArgs.end(); x++)
		addCppLinkArgs.push_back(*x);
	if (other.cppEh)
		cppEh = other.cppEh;
	if (other.cppRtti)
		cppRtti = other.cppRtti;
	if (other.bVSCRT)
		bVSCRT = other.bVSCRT;
}

std::vector<std::vector<std::string> *> CompilerCfg::allCommands() {
	return {
		&c,
		&c_ld,
		&cpp,
		&cpp_ld,
		&ar,
		&windres,
		&strip,
		&cmake,
		&generic,
	};
}
