#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <tchar.h>
#include <windows.h>
#include "test_inspector.hxx"
#include "c_cxxdll.hxx"

Inspector crtexe("crtexe");

/*
 * Initial smoke test
 */

void atexittest() {
	puts("- atexit");
}

int _tmain(int argc, _TCHAR ** argv) {
	Inspector maininspector("_tmain");
	// -- args --
	printf("argc = %i\n", argc);
	printf("sizeof(_TCHAR) = %i\n", (int) sizeof(_TCHAR));
	int i = 0;
	while (i < argc) {
		// skip arg 0 for test consistency
		if (i != 0) {
			printf("argv[%i] = ", i);
			// work around Wine being weirdly 'ANSI-first'
			_TUCHAR * str = (_TUCHAR *) argv[i];
			while (*str) {
				_TUCHAR chr = *str;
				if (chr < 0x80) {
					putchar((int) chr);
				} else {
					printf("<%04x>", chr);
				}
				str++;
			}
			puts("");
		}
		i++;
	}
	// -- atexit --
	puts("+ atexit");
	atexit(atexittest);
	// -- cxxdll --
	Animal * animal = animalFactory();
	animal->detail();
	puts("animal exception test 1");
	// We do a cross-DLL exception here.
	// This is a very important sanity test in the long-term.
	try {
		animalOopsie();
	} catch (std::exception ex) {
		printf("caught: %s\n", ex.what());
	}
	int is_cat = !!dynamic_cast<Cat *>(animal);
	printf("I think the Animal is a Cat, survey says: %i\n", is_cat);
	return 0;
}
