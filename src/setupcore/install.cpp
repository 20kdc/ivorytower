#include "install.h"

using namespace iblis;
using namespace setupcore;

void InstallData::addTarget(CompilerCfg cfg) {
	if (!cfg.machine) {
		IBLIS_WARN("machine without target");
		abort();
	}
	targets.push_back(cfg);
}

Component::Component(const char * name, const char * purpose, bool def) : CvarBool(name, purpose, def) {
}

IBLIS_KIND(Component, "Component");

bool Component::isMeta() {
	return false;
}

class ComponentAll : public Component {
public:
	ComponentAll() : Component("all", "Shorthand to enable/disable all components.", false) {

	}
	bool parse(const char * val) override {
		if (Component::parse(val)) {
			// switch *all* components to this value
			for (Registerable * reg = regFirst; reg; reg = reg->regNext) {
				Component * comp = dynamic_cast<Component *>(reg);
				if (!comp)
					continue;
				comp->value = value;
			}
			return true;
		}
		return false;
	}
	bool isMeta() override {
		return true;
	}
	bool install(InstallData * prepare) override {
		// do absolutely nothing, we're a meta component
		return true;
	}
};

static ComponentAll theComponentAll;
