#include <stdio.h>
#include "iblis.h"

using namespace iblis;

class HelpAct : public Act {
public:
	HelpAct() : Act("--help", "Shows helpful information.") {
	}
	int execute() override {
		printf("ivorytower %s\n", iblis::version);
		puts("");
		puts("Actions:");
		for (Registerable * reg = Registerable::regFirst; reg; reg = reg->regNext) {
			if (!dynamic_cast<Act *>(reg))
				continue;
			printf(" %s: %s\n", reg->name, reg->purpose);
		}
		puts("");
		puts("Components (specified as 'component=on' or 'component=off'):");
		for (Registerable * reg = Registerable::regFirst; reg; reg = reg->regNext) {
			Component * comp = dynamic_cast<Component *>(reg);
			if (!comp)
				continue;
			printf(" %s: %s (currently %s)\n", reg->name, reg->purpose, comp->value ? "on" : "off");
		}
		puts("");
		puts("Options (specified as 'option=value'):");
		for (Registerable * reg = Registerable::regFirst; reg; reg = reg->regNext) {
			Cvar * cvar = dynamic_cast<Cvar *>(reg);
			if (!cvar)
				continue;
			if (dynamic_cast<Component *>(cvar))
				continue;
			auto val = cvar->get();
			printf(" %s: %s (currently '%s')\n", reg->name, reg->purpose, val.c_str());
		}
		puts("");
		return 0;
	}
};

class VersionAct : public Act {
public:
	VersionAct() : Act("--version", "Give version.") {
	}
	int execute() override {
		printf("ivorytower %s\n", iblis::version);
		return 0;
	}
};

class InstallAct : public Act {
public:
	InstallAct() : Act("--install", "Install all enabled components.") {
	}
	int execute() override {
		for (Registerable * reg = Registerable::regFirst; reg; reg = reg->regNext) {
			Component * comp = dynamic_cast<Component *>(reg);
			if (!comp)
				continue;
			if (comp->isMeta())
				continue;
			if (!comp->value)
				continue;
			printf("%s (%s):\n", comp->name, comp->purpose);
			if (!comp->install())
				return 1;
		}
		return 0;
	}
};

static VersionAct theVersionAct;
static HelpAct theHelpAct;
static InstallAct theInstallAct;

int main(int argc, char ** argv) {
	Act * queuedAct = &theHelpAct;
	for (int i = 1; i < argc; i++) {
		char * arg = argv[i];
		char * equ = strchr(arg, '=');
		if (equ) {
			*equ = 0;
			auto cvar = Registerable::find<Cvar>(arg);
			if (cvar) {
				if (!cvar->parse(equ + 1)) {
					printf("Couldn't parse value '%s' for '%s'. Try --help.\n", equ + 1, arg);
					return 1;
				}
			} else {
				printf("Failed to find cvar '%s'. Try --help.\n", arg);
				return 1;
			}
		} else {
			auto act = Registerable::find<Act>(arg);
			if (act) {
				queuedAct = act;
			} else {
				printf("Failed to parse act '%s'. Try --help.\n", arg);
				return 1;
			}
		}
	}
	return queuedAct->execute();
}
