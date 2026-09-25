#include <alloca.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <spawn.h>
#include <unistd.h>
#ifndef __MACH__
#include <wait.h>
#endif

#include "iblis.h"

using namespace iblis;

extern char **environ;

Environ iblis::Environ::readProcess() {
	std::vector<std::string> inner;
	char ** ptr = environ;
	while (*ptr) {
		inner.push_back(*ptr);
		ptr++;
	}
	return Environ {
		.inner = inner
	};
}

void iblis::Environ::del(const std::string & key) {
	for (auto i = inner.begin(); i != inner.end(); i++) {
		// startswith? what's that?
		auto eq = i->find('=');
		if (eq == std::string::npos)
			continue;
		if (eq != key.length())
			continue;
		if (i->substr(0, key.length()) != key)
			continue;
		inner.erase(i);
		return;
	}
	// not found!
}

void iblis::Environ::set(const std::string & key, const std::string & val) {
	del(key);
	inner.push_back(key + "=" + val);
}

static void fireArgTeg(const char ** argva, const std::vector<std::string> & argv) {
	for (size_t i = 0; i < argv.size(); i++)
		argva[i] = argv[i].c_str();
	argva[argv.size()] = nullptr;
}

int iblis::runCmd(const std::vector<std::string> & argv, const Environ & envp) {
	if (argv.size() == 0)
		return -1;
	const char ** argva = (const char **) alloca(sizeof(char *) * (argv.size() + 1));
	fireArgTeg(argva, argv);
	const char ** envpa = (const char **) alloca(sizeof(char *) * (envp.inner.size() + 1));
	fireArgTeg(envpa, envp.inner);
	pid_t pid;
	if (posix_spawnp(&pid, argv[0].c_str(), nullptr, nullptr, (char * const *) argva, (char * const *) envpa))
		return -1;
	while (1) {
		int status;
		if (waitpid(pid, &status, WUNTRACED) == -1)
			return 127;
		if (WIFEXITED(status) || WIFSIGNALED(status))
			return WEXITSTATUS(status);
	}
}

// Writes a file.
bool iblis::writeFile(const std::string & path, const std::string & content) {
	FILE * data = fopen(path.c_str(), "wb");
	if (!data)
		return false;
	fwrite(content.c_str(), content.length(), 1, data);
	fclose(data);
	return true;
}

void iblis::warn(const char * subsystem, const std::string & message) {
	auto tmp = std::string(subsystem) + ": " + message;
	puts(tmp.c_str());
}

HelperSys * iblis::HelperSys::build() {
	const char * itsetupDirEnv = getenv("ITSETUP_DIR");
	if (!itsetupDirEnv) {
		IBLIS_WARN("Couldn't initialize, unexpected missing ITSETUP_DIR");
		return nullptr;
	}
	return new HelperSys(itsetupDirEnv);
}

std::string iblis::HelperSys::helper(const char * name) {
	return itsetupDir + "/helpers/" + name;
}

std::string iblis::HelperSys::osxcrossBinLink() {
	// A symlink is placed here. That symlink points to osxcross/target/bin OR potentially to something like /usr/local/bin.
	// This allows portable osxcross installs and some hypothetical future global osxcross install to be used.
	return itsetupDir + "/osxcross_bin";
}

Subsystem<HelperSys> iblis::helperSys;
