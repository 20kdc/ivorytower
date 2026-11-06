#include <stdio.h>
// #include <iostream>

// from mylibrary.cpp
#ifdef _WIN32
#define MYLIBRARY_API __declspec(dllimport)
#else
#define MYLIBRARY_API
#endif
int MYLIBRARY_API add(int a, int b);

class Hello {
public:
	Hello() {
		// lambda test
		[] {
			puts("Hello, world!");
			add(12, 23);
		}();
	}
};

Hello hello;

int main() {
	// std::cout << "THIS SHOULD NOT PASS\n";
	return 0;
}
