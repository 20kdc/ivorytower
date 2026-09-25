#include <stdio.h>
#include "common/iblis.h"
#include "install.h"

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
			if (!comp->install()) {
				puts("");
				printf("Component %s (%s) failed.\nYou may correct the issue and retry, or disable this component by passing `%s=off`.\n", reg->name, reg->purpose, reg->name);
				return 1;
			}
		}
		return 0;
	}
};

static HelpAct theHelpAct;
static InstallAct theInstallAct;

Act * iblis::theDefaultAct = &theHelpAct;
