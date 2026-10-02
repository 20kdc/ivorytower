#include "common/iblis.h"

namespace iblis {
	class W32CrossSys {
	public:
		IBLIS_IMMOVABLE(W32CrossSys)
		static W32CrossSys * build(const char * sdk);
		std::string prefix;
		std::string toolPath(const std::string & tool);
		std::vector<std::string> getClangArgs(const std::string & config);
	private:
		W32CrossSys(std::string prefix) : prefix(prefix) {}
	};
	extern Subsystem<W32CrossSys> w32CrossSys_w10;
}
