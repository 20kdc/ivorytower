#include "common/iblis.h"

namespace iblis {
	class W32CrossSys {
	public:
		IBLIS_IMMOVABLE(W32CrossSys)
		static W32CrossSys * build(const char * sdk);
		std::string prefix;
	private:
		W32CrossSys(std::string prefix) : prefix(prefix) {}
	};
	extern Subsystem<W32CrossSys> w32CrossSys_w10;
}
