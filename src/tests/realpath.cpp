#include <assert.h>
#include <unistd.h>
#include "common/iblis.h"

void trial(std::string in, std::string out) {
	std::string c = iblis::realPath(in);
	fprintf(stderr, "'%s' -> '%s' = '%s'\n", in.c_str(), c.c_str(), out.c_str());
	if (c != out) {
		abort();
	}
}

int main(int argc, char ** argv) {
	chdir("..");
	trial("/__IBLIS_DOESNOTEXIST", "/__IBLIS_DOESNOTEXIST");
	trial("./__IBLIS_DOESNOTEXIST", std::string(get_current_dir_name()) + "/__IBLIS_DOESNOTEXIST");
	trial("src/tests/realpath.cpp", std::string(get_current_dir_name()) + "/src/tests/realpath.cpp");
	trial("src/tests/realpath_util/link1", std::string(get_current_dir_name()) + "/src/tests/realpath.cpp");
	trial("src/tests/realpath_util/link2/realpath_util/fake_file", std::string(get_current_dir_name()) + "/src/tests/realpath_util/fake_file");
	return 0;
}
