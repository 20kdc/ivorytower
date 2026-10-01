/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

/* We could define WIN32_LEAN_AND_MEAN here, but it's more useful to exercise the includes. */
#include <windows.h>
#include "crt0_common.h"

int __stdcall DllMain(void * a, int reason, void * reserved);

int __stdcall _CRT_INIT(void * a, int reason, void * reserved) {
	static int counter = 0;
	if (reason == DLL_PROCESS_ATTACH) {
		counter++;
		return __NOTVCRUNTIME_init(1);
	} else if (reason == DLL_PROCESS_DETACH) {
		counter--;
		if (counter == 0) {
			__NOTVCRUNTIME_dll_fini();
			return 0;
		}
	}
	return 1;
}

int __stdcall _DllMainCRTStartup(void * a, int reason, void * reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		if (!_CRT_INIT(a, reason, reserved))
			return 0;
		return DllMain(a, reason, reserved);
	} else if (reason == DLL_PROCESS_DETACH) {
		DllMain(a, reason, reserved);
		return _CRT_INIT(a, reason, reserved);
	} else {
		return DllMain(a, reason, reserved);
	}
}
