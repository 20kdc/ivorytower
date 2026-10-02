// contains useful 'dummy' functions
#include <windows.h>
#include <setjmp.h>

// NOLINTNEXTLINE
__declspec(dllexport) void it_vcruntime140() {
	jmp_buf buf;
	setjmp(buf);
	Sleep(0);
	longjmp(buf, 1);
}
