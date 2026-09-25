#include <stdio.h>
#include "common/iblis.h"
#include "install.h"

using namespace iblis;

class ComponentHelpCategory : public HelpCategory {
public:
	ComponentHelpCategory() : HelpCategory("components", "Components (specified as 'component=on' or 'component=off')") {
	}
	void execute() override {
		for (Registerable * reg = Registerable::regFirst; reg; reg = reg->regNext) {
			if (reg->getKind() != Component::kind)
				continue;
			Component * comp = static_cast<Component *>(reg);
			printf(" %s: %s (currently %s)\n", reg->name, reg->purpose, comp->value ? "on" : "off");
		}
	}
};
static ComponentHelpCategory theComponentHelpCategory;

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

static InstallAct theInstallAct;

int main(int argc, char ** argv) {
	return iblis::main(argc, argv);
}
