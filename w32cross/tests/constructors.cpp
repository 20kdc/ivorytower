#include <stdio.h>

class Inspector {
public:
	Inspector() {
		puts("Inspector created.");
	}
	~Inspector() {
		puts("Inspector destroyed.");
	}
};

Inspector i;

int main(int argc, char ** argv) {
	return 0;
}
