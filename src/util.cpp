#include <alloca.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <spawn.h>
#include <unistd.h>
#include <wait.h>

#include "iblis.h"

int iblis::runCmd(const std::string & cmd, const std::vector<std::string> & args) {
	const char ** argv = (const char **) alloca(sizeof(char *) * (args.size() + 2));
	size_t p = 0;
	argv[p++] = cmd.c_str();
	for (size_t i = 0; i < args.size(); i++)
		argv[p++] = args[i].c_str();
	argv[p++] = nullptr;
	pid_t pid;
	if (posix_spawnp(&pid, cmd.c_str(), nullptr, nullptr, (char * const *) argv, environ))
		return 127;
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
