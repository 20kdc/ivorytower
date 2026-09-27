/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#include "nvcr_con.h"

int __stdcall DllMain(void * a, int reason, void * c);

int __stdcall _DllMainCRTStartup(void * a, int reason, void * c) {
	/* TODO: Responsibly init the CRT */
	return DllMain(a, reason, c);
}

#pragma comment(linker, "/alternatename:DllMain=_DllMainCRTStartup_Default")
int __stdcall _DllMainCRTStartup_Default(void * a, int reason, void * c) {
	return 1;
}
