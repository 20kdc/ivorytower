#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <exception>

#define ANIMALDECL __declspec(dllexport)
#include "c_cxxdll.hxx"

void Animal::detail() {
	printf("The %s says: ", typeid(*this).name());
	voice();
}

Animal::~Animal() {
}

Cat::Cat() {
}

void Cat::voice() {
	puts("Meow~!");
}

ANIMALDECL Animal * animalFactory() {
	return new Cat();
}

ANIMALDECL void animalOopsie() {
	throw std::exception("oopsie!");
}

void atexittest() {
	puts("Hi, I'm an atexit function living in a DLL.");
}

// This is to confirm the real DllMain takes precedence.
int __stdcall DllMain(void * a, int reason, void * c) {
	if (reason == DLL_PROCESS_ATTACH) {
		atexit(atexittest);
		puts("Hi, I'm the real DllMain, and I just got attached.");
	} else {
		puts("Hi, I'm the real DllMain, and something else happened.");
	}
	return 1;
}
