#include <windows.h>
#include "../b_launch.hxx"
#include "../test_inspector.hxx"

__declspec(thread) Inspector myInspector("example");

// temporary stuff, NYI
extern "C" _stdcall void __dyn_tls_init(int a, int b, int c) {}
extern "C" void __tlregdtor() {}
extern "C" {
	int _tls_index;
	void * _tls_array;
}

WINAPI unsigned long myThreadFn(void *) {
	// do nothing
	return 0;
}

TAPTESTSEQ_OPEN
	HANDLE myThread = CreateThread(NULL, 0, myThreadFn, NULL, 0, NULL);
	WaitForSingleObject(myThread, INFINITE);
	CloseHandle(myThread);
TAPTESTSEQ_CLOSE
