#include <stdio.h>
#include <tchar.h>

int _tmain(int argc, _TCHAR ** argv) {
	printf("argc = %i\n", argc);
	int i = 0;
	while (i < argc) {
#ifndef _UNICODE
		printf("argv[%i] = %s\n", i, argv[i]);
#else
		printf("argv[%i] = %S\n", i, argv[i]);
#endif
		i++;
	}
	return 0;
}
