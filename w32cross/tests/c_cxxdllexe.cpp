#include <stdio.h>
#include "c_cxxdll.hxx"

void atexittest() {
	puts("Hi, I'm an atexit function living in an EXE.");
}

int main(int argc, char ** argv) {
	atexit(atexittest);
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
	printf("I think the Animal is a Cat, survey says: %p\n", dynamic_cast<Cat *>(animal));
	return 0;
}
