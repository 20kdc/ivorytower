#include <stdio.h>
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

// This is to confirm the real DllMain takes precedence.
int __stdcall DllMain(void * a, int reason, void * c) {
	puts("Hi, I'm the real DllMain.");
	return 1;
}
