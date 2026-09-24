#include "names.h"

/* machine definitions */

const iblis::Machine iblis::Machine::lgx32 = {
	.ivtName = "lgx32",

	.system = "linux",
	.cpu_family = "x86",
	.cpu = "i686",
	.endian = "little",
};
const iblis::Machine iblis::Machine::lgx64 = {
	.ivtName = "lgx64",

	.system = "linux",
	.cpu_family = "x86_64",
	.cpu = "x86_64",
	.endian = "little",
};
const iblis::Machine iblis::Machine::lga64 = {
	.ivtName = "lga64",

	.system = "linux",
	.cpu_family = "aarch64",
	.cpu = "aarch64",
	.endian = "little",
};

const iblis::Machine iblis::Machine::mx32 = {
	.ivtName = "mx32",

	.system = "darwin",
	.subsystem = "macos",
	.kernel = "xnu",
	.cpu_family = "x86",
	.cpu = "i686",
	.endian = "little",
};
const iblis::Machine iblis::Machine::mx64 = {
	.ivtName = "mx64",

	.system = "darwin",
	.subsystem = "macos",
	.kernel = "xnu",
	.cpu_family = "x86_64",
	.cpu = "x86_64",
	.endian = "little",
};
const iblis::Machine iblis::Machine::ma64 = {
	.ivtName = "ma64",

	.system = "darwin",
	.subsystem = "macos",
	.kernel = "xnu",
	.cpu_family = "aarch64",
	.cpu = "aarch64",
	.endian = "little",
};

const iblis::Machine iblis::Machine::wx32 = {
	.ivtName = "wx32",

	.system = "windows",
	.subsystem = "windows",
	.kernel = "nt",
	.cpu_family = "x86",
	.cpu = "i686",
	.endian = "little",
};
const iblis::Machine iblis::Machine::wx64 = {
	.ivtName = "wx64",

	.system = "windows",
	.subsystem = "windows",
	.kernel = "nt",
	.cpu_family = "x86_64",
	.cpu = "x86_64",
	.endian = "little",
};
const iblis::Machine iblis::Machine::wa64 = {
	.ivtName = "wa64",

	.system = "windows",
	.subsystem = "windows",
	.kernel = "nt",
	.cpu_family = "aarch64",
	.cpu = "aarch64",
	.endian = "little",
};

const iblis::STLDisposition iblis::STLDisposition::none = {
	.ivtName = "none",
	.compilerArgs = {
		.addCArgs = {"-fno-exceptions"},
		.addCLinkArgs = {"-nostdlib++"},
		.addCppArgs = {"-nostdinc++", "-fno-rtti", "-fno-exceptions"},
		.addCppLinkArgs = {"-nostdlib++"},
		.cppEh = "none",
		.cppRtti = "false",
	}
};

const iblis::STLDisposition iblis::STLDisposition::stl = {
	.ivtName = "stl",
	.compilerArgs = {
		.addCArgs = {},
		.addCLinkArgs = {},
		.addCppArgs = {},
		.addCppLinkArgs = {},
	}
};

const iblis::STLDisposition iblis::STLDisposition::staticstl = {
	.ivtName = "staticstl",
	.compilerArgs = {
		.addCArgs = {},
		.addCLinkArgs = {},
		.addCppArgs = {},
		.addCppLinkArgs = {"-static-libstdc++"},
	}
};

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
