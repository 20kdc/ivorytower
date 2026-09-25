#include "install.h"

using namespace iblis;

Component::Component(const char * name, const char * purpose, bool def) : CvarBool(name, purpose, def) {
}

IBLIS_KIND(Component, "Component");

bool Component::isMeta() {
	return false;
}

class ComponentAll : public iblis::Component {
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
	bool install() override {
		// do absolutely nothing, we're a meta component
		return true;
	}
};

static ComponentAll theComponentAll;
