#pragma once

#include <stdio.h>

class Inspector {
public:
	const char * name;
	int isAlive;
	Inspector(const char * name) : name(name), isAlive(0) {
		printf("+ Inspector(%s)\n", name);
		isAlive = 1;
	}
	~Inspector() {
		isAlive = 0;
		printf("- Inspector(%s)\n", name);
	}
};
