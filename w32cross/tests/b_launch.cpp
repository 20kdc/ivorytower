#include <windows.h>
#include "b_launch.hxx"

class TapTestEnvImpl : public TapTestEnv {
public:
	virtual void observe(void * ptr) override {
		// do nothing, but the test doesn't know that
	}
};

TapTestEnvImpl testEnvironment;

void launch_test(const char * testid) {
	printf("\n%s {\n", testid);
	HMODULE lib = LoadLibraryA(testid);
	if (!lib) {
		printf("MISSING: %s\n", testid);
		exit(1);
	}
	TapTestSeq ** testptr = (TapTestSeq **) GetProcAddress(lib, "X_4AE");
	if (!testptr) {
		printf("MISSING TESTPTR: %s\n", testid);
		exit(1);
	}
	(*testptr)->invoke(&testEnvironment);
	FreeLibrary(lib);
	printf("}\n\n");
}

int main(int argc, char ** argv) {
	launch_test("mod_exceptions_vs_destructors");
	launch_test("mod_cout");
	launch_test("mod_builtins");
	launch_test("mod_declspec_thread");
}
