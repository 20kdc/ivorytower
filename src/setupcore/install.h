#include "common/iblis.h"
#include "names.h"

namespace setupcore {
	class InstallData {
	public:
		InstallData() {}
		IBLIS_IMMOVABLE(InstallData);
		void addTarget(CompilerCfg cfg);
		std::vector<CompilerCfg> getTargets() {
			return targets;
		}
	private:
		std::vector<CompilerCfg> targets;
	};
	class Component : public iblis::CvarBool {
	public:
		Component(const char * name, const char * purpose, bool def);
		IBLIS_HAS_KIND;
		// 'meta component' flag (false, is for "all" only)
		virtual bool isMeta();
		virtual bool install(InstallData * iData) = 0;
	};
}
