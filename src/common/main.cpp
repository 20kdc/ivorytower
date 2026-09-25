#include "iblis.h"

using namespace iblis;

int main(int argc, char ** argv) {
	Act * queuedAct = theDefaultAct;
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
