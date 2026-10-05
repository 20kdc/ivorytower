#include <stdio.h>

#ifdef _WIN32
#define MYLIBRARY_API __declspec(dllexport)
#else
#define MYLIBRARY_API
#endif

int MYLIBRARY_API add(int a, int b) {
	printf("%i + %i = %i\n", a, b, a + b);
	return a + b;
}
