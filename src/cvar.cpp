#include <stdlib.h>
#include "iblis.h"

using namespace iblis;

Registerable * Registerable::regFirst = nullptr;

Registerable::Registerable(const char * name, const char * purpose) : name(name), purpose(purpose) {
	Registerable ** insertRef;
	for (insertRef = &regFirst; *insertRef; insertRef = &(*insertRef)->regNext) {
		// nothing for now
		if (strcmp(name, (*insertRef)->name) <= 0)
			break;
	}
	// insert
	regNext = *insertRef;
	*insertRef = this;
}

Registerable::~Registerable() {
	// technically, this should probably unlink the Registerable, but we don't.
}

Act::Act(const char * name, const char * purpose) : Registerable(name, purpose) {
}

Cvar::Cvar(const char * name, const char * purpose) : Registerable(name, purpose) {
}

CvarBool::CvarBool(const char * name, const char * purpose, bool def) : Cvar(name, purpose), value(def) {
}

static bool boolParseInner(bool * value, const char * val) {
	if ((!strcmp(val, "off")) || (!strcmp(val, "0"))) {
		*value = false;
		return true;
	}
	if ((!strcmp(val, "on")) || (!strcmp(val, "1"))) {
		*value = true;
		return true;
	}
	return false;
}

bool CvarBool::parse(const char * val) {
	return boolParseInner(&value, val);
}

std::string CvarBool::get() {
	return std::string(value ? "1" : "0");
}

CvarStr::CvarStr(const char * name, const char * purpose, std::string def) : Cvar(name, purpose), value(def) {
}

bool CvarStr::parse(const char * val) {
	value = val;
	return true;
}

std::string CvarStr::get() {
	return value;
}

Component::Component(const char * name, const char * purpose, bool def) : CvarBool(name, purpose, def) {
}

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

ComponentAll theComponentAll;
