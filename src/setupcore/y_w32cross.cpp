#include "y_w32cross.h"
#include <unistd.h>

using namespace iblis;

Subsystem<W32CrossSys> iblis::w32CrossSys_w10("w10");

W32CrossSys * W32CrossSys::build(const char * sdk) {
	HelperSys * helper = helperSys.get();
	if (!helper) {
		IBLIS_WARN("Couldn't initialize, HelperSys dead.");
		return nullptr;
	}
	if (iblis::runCmd({helper->helper("w32cross-wizard"), sdk}) == 0) {
		return new W32CrossSys(helper->w32crossSDK(sdk));
	}
	return nullptr;
}

std::string W32CrossSys::toolPath(const std::string & tool) {
	return {prefix + "/bin/" + tool};
}

std::vector<std::string> W32CrossSys::getClangArgs(const std::string & config) {
	std::vector<std::string> unfiltered = readFile(prefix + "/etc/clang-args/" + config + ".lst");
	std::vector<std::string> filtered;
	for (auto path : unfiltered) {
		std::string path2 = path;
		size_t pos = 0;
		while (true) {
			auto res = path2.find("${W32CROSS_SDKROOT}", pos);
			if (res == std::string::npos)
				break;
			path2 = path2.replace(res, 19, prefix);
			pos = res + prefix.length();
		}
		if (!path2.empty())
			filtered.push_back(path2);
	}
	return filtered;
}
