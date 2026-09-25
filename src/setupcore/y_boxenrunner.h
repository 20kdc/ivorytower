#include "common/iblis.h"

namespace iblis {
	class BoxenrunnerSys {
	public:
		IBLIS_IMMOVABLE(BoxenrunnerSys)
		static BoxenrunnerSys * build();
		bool create(const std::string & container, const std::string & image);
		bool test(const std::string & container);
		std::vector<std::string> prefix(const std::string & container);
	private:
		std::string helper, runner;
		BoxenrunnerSys(std::string helper, std::string runner) : helper(helper), runner(runner) {}
	};
	extern Subsystem<BoxenrunnerSys> boxenrunnerSys;
	// BoxenrunnerSys will silently fail if this is true; code may need to be aware of this.
	extern CvarBool cvar_box_create;
}
