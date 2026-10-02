/*
 * The purpose of this test is to include and link with whatever we can.
 * This sanity-checks that a lot of critical runtime functions can be found and linked to.
 */

#include "endlesschamber/stl.h"
#include "endlesschamber/vcruntime140.hxx"
#include "endlesschamber/concrt140.hxx"

__declspec(dllexport) int narwhals() {
	static int narwhals = 0;
	return narwhals++;
}
