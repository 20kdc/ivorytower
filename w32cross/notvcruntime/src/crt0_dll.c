/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

/* We could define WIN32_LEAN_AND_MEAN here, but it's more useful to exercise the includes. */
#include <windows.h>

#include "corecrt_startup.h"
#include "crt0_common.h"

int __stdcall DllMain(void * a, int reason, void * c);

int __stdcall _DllMainCRTStartup(void * a, int reason, void * c) {
	if (reason == DLL_PROCESS_ATTACH) {
		if (__NOTVCRUNTIME_init())
			return 0;
		return DllMain(a, reason, c);
	} else if (reason == DLL_PROCESS_DETACH) {
		int val = DllMain(a, reason, c);
		__NOTVCRUNTIME_fini();
		return val;
	} else {
		return DllMain(a, reason, c);
	}
}

#pragma comment(linker, "/alternatename:DllMain=__NOTVCRUNTIME__DllMainCRTStartup_Default")
