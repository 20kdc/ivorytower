#include <stdio.h>

int main(int argc, char ** argv) {
	printf("argc = %i\n", argc);
	int i = 0;
	while (i < argc)
		printf("argv[%i] = %s\n", i, argv[i]);
	return 0;
}
