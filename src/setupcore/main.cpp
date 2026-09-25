#include <stdio.h>
#include "common/iblis.h"
#include "install.h"
#include "meson.h"

using namespace iblis;
using namespace setupcore;

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
		InstallData iData;
		for (Registerable * reg = Registerable::regFirst; reg; reg = reg->regNext) {
			if (reg->getKind() != Component::kind)
				continue;
			Component * comp = static_cast<Component *>(reg);
			if (!comp)
				continue;
			if (comp->isMeta())
				continue;
			if (!comp->value)
				continue;
			printf("%s (%s):\n", comp->name, comp->purpose);
			if (!comp->install(&iData)) {
				puts("");
				printf("Component %s (%s) failed.\nYou may correct the issue and retry, or disable this component by passing `%s=off`.\n", reg->name, reg->purpose, reg->name);
				return 1;
			}
		}
		puts("All component pre-setup completed; installing crossfiles...");
		bool ok = true;
		auto compilers = iData.getTargets();
		for (auto i = compilers.begin(); i != compilers.end(); i++) {
			ok &= meson::installCrossFiles(*i);
		}
		return ok ? 0 : 1;
	}
};

static InstallAct theInstallAct;

int main(int argc, char ** argv) {
	return iblis::main(argc, argv);
}
