/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#define _DECL_DLLMAIN

#include <process.h>

#include "nvcr_con.h"

int __stdcall _DllMainCRTStartup(void * a, int reason, void * c) {
	return _CRT_INIT(a, reason, c);
}
