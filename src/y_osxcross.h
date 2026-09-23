#include "iblis.h"

namespace iblis {
	class OSXCrossSys {
	public:
		IBLIS_IMMOVABLE(OSXCrossSys)
		static OSXCrossSys * build();
		std::string toolPath(const std::string & tool);
	private:
		std::string prefix;
		OSXCrossSys(std::string prefix) : prefix(prefix) {}
	};
	extern Subsystem<OSXCrossSys> osxCrossSys;
}
