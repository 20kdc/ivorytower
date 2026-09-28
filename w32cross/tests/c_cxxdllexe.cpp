#include <stdio.h>
#include "c_cxxdll.hxx"

int main(int argc, char ** argv) {
	Animal * animal = animalFactory();
	animal->detail();
	puts("animal exception test 1");
	// We do a cross-DLL exception here.
	// This is a very important sanity test in the long-term.
	try {
		animalOopsie();
	} catch (std::exception ex) {
		printf("caught: %s\n", ex.what());
	}
	return 0;
}
