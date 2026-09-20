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

bool CvarBool::parse(const char * val) {
	if ((!strcmp(val, "off")) || (!strcmp(val, "0"))) {
		value = false;
		return true;
	}
	if ((!strcmp(val, "on")) || (!strcmp(val, "1"))) {
		value = true;
		return true;
	}
	return false;
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
