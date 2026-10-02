#include "endlesschamber/stl.h"

__declspec(dllexport) int narwhals() {
	static int narwhals = 0;
	return narwhals++;
}
