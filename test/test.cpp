#include <stdio.h>
// #include <iostream>

class Hello {
public:
	Hello() {
		// lambda test
		[] {
			puts("Hello, world!");
		}();
	}
};

Hello hello;

int main() {
	// std::cout << "THIS SHOULD NOT PASS\n";
	return 0;
}
