#include "iblis.h"

using namespace iblis;

class VersionAct : public Act {
public:
	VersionAct() : Act("version", "", "Give version.") {
	}
	int execute(const std::vector<const char *> & args) override {
		printf("ivorytower %s\n", iblis::version);
		return 0;
	}
};
static VersionAct theVersionAct;

static const char * cmdNameVar = "?";

class HelpAct : public Act {
public:
	HelpAct() : Act("help", "", "Shows helpful information.") {
	}
	int execute(const std::vector<const char *> & args) override {
		printf("ivorytower %s\n", iblis::version);
		printf("Usage: %s --ACT <--example=VALUE...>\n", cmdNameVar);
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
			Act * act = (Act *) reg;
			printf(" --%s%s: %s\n", reg->name, act->argsum, reg->purpose);
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
			printf(" --%s=%s: %s (currently '%s')\n", reg->name, cvar->valdesc, reg->purpose, val.c_str());
		}
		return;
	}
};
static CvarHelpCategory theCvarHelpCategory;

int iblis::main(const char * cmdName, int argc, char ** argv) {
	cmdNameVar = cmdName;
	Act * queuedAct = &theHelpAct;
	std::vector<const char *> args;
	bool forcePositional = false;
	for (int i = 1; i < argc; i++) {
		char * arg = argv[i];
		if ((!forcePositional) && (arg[0] == '-' && arg[1] == '-')) {
			if (arg[2] == 0) {
				forcePositional = true;
			} else {
				char * argNeq = arg + 2;
				char * equ = strchr(argNeq, '=');
				if (equ) {
					*equ = 0;
					auto cvar = Registerable::find<Cvar>(argNeq);
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
					auto act = Registerable::find<Act>(argNeq);
					if (act) {
						queuedAct = act;
					} else {
						printf("Failed to parse act '%s'. Try --help.\n", arg);
						return 1;
					}
				}
			}
		} else {
			args.push_back(arg);
		}
	}
	if (!queuedAct->checkArgc(args.size())) {
		printf("--%s%s: %i args incorrect\n", queuedAct->name, queuedAct->argsum, (int) args.size());
		return 1;
	}
	return queuedAct->execute(args);
}
