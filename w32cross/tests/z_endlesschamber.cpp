#include "endlesschamber/stl.h"
#include "endlesschamber/vcruntime140.hxx"
#include "endlesschamber/concrt140.hxx"

__declspec(dllexport) int narwhals() {
	static int narwhals = 0;
	return narwhals++;
}
