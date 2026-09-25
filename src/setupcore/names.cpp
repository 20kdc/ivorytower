#include "names.h"

using namespace iblis;

/* CPU defintiions */

const CPU CPU::x32("x86", "i686", "little");
const CPU CPU::x64("x86_64", "x86_64", "little");
const CPU CPU::a64("aarch64", "aarch64", "little");

/* OS defintiions */

const OS OS::lg("linux", nullptr, "linux");
const OS OS::m("darwin", "macos", "xnu");
const OS OS::w("windows", "windows", "nt");

/* machine definitions */

const Machine Machine::lgx32("lgx32", &OS::lg, &CPU::x32);
const Machine Machine::lgx64("lgx64", &OS::lg, &CPU::x64);
const Machine Machine::lga64("lga64", &OS::lg, &CPU::a64);

const Machine Machine::mx32("mx32", &OS::m, &CPU::x32);
const Machine Machine::mx64("mx64", &OS::m, &CPU::x64);
const Machine Machine::ma64("ma64", &OS::m, &CPU::a64);

const Machine Machine::wx32("wx32", &OS::w, &CPU::x32);
const Machine Machine::wx64("wx64", &OS::w, &CPU::x64);
const Machine Machine::wa64("wa64", &OS::w, &CPU::a64);

const iblis::STLDisposition iblis::STLDisposition::none = [] {
	iblis::STLDisposition result;
	result.ivtName = "none";
	result.compilerArgs.addCArgs = {"-fno-exceptions"};
	result.compilerArgs.addCLinkArgs = {"-nostdlib++"};
	result.compilerArgs.addCppArgs = {"-nostdinc++", "-fno-rtti", "-fno-exceptions"};
	result.compilerArgs.addCppLinkArgs = {"-nostdlib++"};
	result.compilerArgs.cppEh = "none";
	result.compilerArgs.cppRtti = "false";
	return result;
}();

const iblis::STLDisposition iblis::STLDisposition::stl = [] {
	iblis::STLDisposition result;
	result.ivtName = "stl",
	result.compilerArgs.addCArgs = {};
	result.compilerArgs.addCLinkArgs = {};
	result.compilerArgs.addCppArgs = {};
	result.compilerArgs.addCppLinkArgs = {};
	return result;
}();

const iblis::STLDisposition iblis::STLDisposition::staticstl = [] {
	iblis::STLDisposition result;
	result.ivtName = "staticstl",
	result.compilerArgs.addCArgs = {};
	result.compilerArgs.addCLinkArgs = {};
	result.compilerArgs.addCppArgs = {};
	result.compilerArgs.addCppLinkArgs = {"-static-libstdc++"};
	return result;
}();

void iblis::CompilerArgs::merge(const CompilerArgs & other) {
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
}

std::vector<std::vector<std::string> *> iblis::CompilerCfg::allCommands() {
	return {
		&c,
		&cpp,
		&ar,
		&windres,
		&strip
	};
}
