#include "common/iblis.h"

namespace iblis {
	class OSXCrossSys {
	public:
		IBLIS_IMMOVABLE(OSXCrossSys)
		static OSXCrossSys * build();
		std::string toolPath(const std::string & tool);
		std::string archToolPath(const std::string & arch, const std::string & tool);
	private:
		std::string prefix, target;
		OSXCrossSys(std::string prefix, std::string target) : prefix(prefix), target(target) {}
	};
	extern Subsystem<OSXCrossSys> osxCrossSys;
}
