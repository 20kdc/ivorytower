#include <windows.h>
#include "../b_launch.hxx"
#include "../test_inspector.hxx"

__declspec(thread) Inspector myInspector("example");

// temporary stuff, NYI
// https://github.com/llvm/llvm-project/blob/76e574c106cd4ae67ca6e0f6f44ea551e77212f5/clang/lib/CodeGen/MicrosoftCXXABI.cpp#L2451
extern "C" _stdcall void __dyn_tls_init(int a, int b, int c) {
}
// https://github.com/llvm/llvm-project/blob/76e574c106cd4ae67ca6e0f6f44ea551e77212f5/clang/lib/CodeGen/MicrosoftCXXABI.cpp#L2414
extern "C" int __tlregdtor(void (*f)()) {
	return 0;
}
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
