/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

/* We could define WIN32_LEAN_AND_MEAN here, but it's more useful to exercise the includes. */
#include <windows.h>

#include "nvcr_con.h"

int __stdcall DllMain(void * a, int reason, void * c);

int __stdcall _DllMainCRTStartup(void * a, int reason, void * c) {
	if (reason == DLL_PROCESS_ATTACH) {
		if (_initterm_e(__xi_a, __xi_z))
			return 0;
		_initterm(__xc_a, __xc_z);
		return DllMain(a, reason, c);
	} else if (reason == DLL_PROCESS_DETACH) {
		int val = DllMain(a, reason, c);
		_initterm(__xt_a, __xt_z);
		return val;
	} else {
		return DllMain(a, reason, c);
	}
}

#pragma comment(linker, "/alternatename:DllMain=_DllMainCRTStartup_Default")
int __stdcall _DllMainCRTStartup_Default(void * a, int reason, void * c) {
	return 1;
}
