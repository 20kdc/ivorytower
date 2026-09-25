#include "iblis.h"

using namespace iblis;

class VersionAct : public Act {
public:
	VersionAct() : Act("--version", "Give version.") {
	}
	int execute() override {
		printf("ivorytower %s\n", iblis::version);
		return 0;
	}
};
static VersionAct theVersionAct;

class HelpAct : public Act {
public:
	HelpAct() : Act("--help", "Shows helpful information.") {
	}
	int execute() override {
		printf("ivorytower %s\n", iblis::version);
		puts("Usage: ./setup --ACT <example=VALUE...>");
		puts("If multiple ACTs are specified, last wins.");
		for (Registerable * reg = Registerable::regFirst; reg; reg = reg->regNext) {
			if (reg->getKind() != HelpCategory::kind)
				continue;
			puts("");
			printf("%s:\n", reg->purpose);
			static_cast<HelpCategory *>(reg)->execute();
		}
		return 0;
	}
};
static HelpAct theHelpAct;

class ActHelpCategory : public HelpCategory {
public:
	ActHelpCategory() : HelpCategory("actions", "Actions") {
	}
	void execute() override {
		for (Registerable * reg = Registerable::regFirst; reg; reg = reg->regNext) {
			if (reg->getKind() != Act::kind)
				continue;
			printf(" %s: %s\n", reg->name, reg->purpose);
		}
		return;
	}
};
static ActHelpCategory theActHelpCategory;

class CvarHelpCategory : public HelpCategory {
public:
	CvarHelpCategory() : HelpCategory("options", "Options") {
	}
	void execute() override {
		for (Registerable * reg = Registerable::regFirst; reg; reg = reg->regNext) {
			if (reg->getKind() != Cvar::kind)
				continue;
			Cvar * cvar = static_cast<Cvar *>(reg);
			auto val = cvar->get();
			printf(" %s: %s (currently '%s')\n", reg->name, reg->purpose, val.c_str());
		}
		return;
	}
};
static CvarHelpCategory theCvarHelpCategory;

int iblis::main(int argc, char ** argv) {
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
