#include "common/iblis.h"

namespace iblis {
	class W32CrossSys {
	public:
		IBLIS_IMMOVABLE(W32CrossSys)
		static W32CrossSys * build();
		std::string toolPath(const std::string & tool);
	private:
		std::string prefix;
		W32CrossSys(std::string prefix) : prefix(prefix) {}
	};
	extern Subsystem<W32CrossSys> w32CrossSys;
}
