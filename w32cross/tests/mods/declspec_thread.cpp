#include <windows.h>
#include "../b_launch.hxx"
#include "../test_inspector.hxx"

__declspec(thread) Inspector myInspector("example");
__declspec(thread) int counter = 1;

WINAPI unsigned long myThreadFn(void *) {
	counter++;
	printf("\ttls counter %x == 2\n", counter);
	return 0;
}

TAPTESTSEQ_OPEN
	for (int i = 0; i < 2; i++) {
		HANDLE myThread = CreateThread(NULL, 0, myThreadFn, NULL, 0, NULL);
		WaitForSingleObject(myThread, INFINITE);
		CloseHandle(myThread);
	}
TAPTESTSEQ_CLOSE
