#include "common/iblis.h"

namespace iblis {
	class Component : public CvarBool {
	public:
		Component(const char * name, const char * purpose, bool def);
		// 'meta component' flag (false, is for "all" only)
		virtual bool isMeta();
		virtual bool install() = 0;
	};
}
