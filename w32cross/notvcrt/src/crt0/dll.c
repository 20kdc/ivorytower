/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

/* We could define WIN32_LEAN_AND_MEAN here, but it's more useful to exercise the includes. */

#include "crt0_common.h"

int __stdcall DllMain(HMODULE * module, int reason, void * reserved);

int __stdcall _CRT_INIT(HMODULE * module, int reason, void * reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		__NOTVCRUNTIME_tlsemu(module, reason, reserved);
		return __NOTVCRUNTIME_dll_init();
	} else if (reason == DLL_PROCESS_DETACH) {
		__NOTVCRUNTIME_dll_fini();
		__NOTVCRUNTIME_tlsemu(module, reason, reserved);
		return 0;
	} else if (reason == DLL_THREAD_ATTACH) {
		__NOTVCRUNTIME_tlsemu(module, reason, reserved);
	} else if (reason == DLL_THREAD_DETACH) {
		__NOTVCRUNTIME_tlsemu(module, reason, reserved);
	}
	return 1;
}

int __stdcall _DllMainCRTStartup(HMODULE * module, int reason, void * reserved) {
	if (reason == DLL_PROCESS_ATTACH || reason == DLL_THREAD_ATTACH) {
		if (!_CRT_INIT(module, reason, reserved))
			return 0;
		return DllMain(module, reason, reserved);
	} else if (reason == DLL_PROCESS_DETACH || reason == DLL_THREAD_DETACH) {
		DllMain(module, reason, reserved);
		return _CRT_INIT(module, reason, reserved);
	} else {
		return DllMain(module, reason, reserved);
	}
}
