#pragma once

class Inspector {
public:
	const char * name;
	Inspector(const char * name) : name(name) {
		printf("+ Inspector(%s)\n", name);
	}
	~Inspector() {
		printf("- Inspector(%s)\n", name);
	}
};
