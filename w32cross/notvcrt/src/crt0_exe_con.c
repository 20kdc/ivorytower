/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>

#include "crt0_common.h"

int _tmain(int argc, _TCHAR ** argv);

#ifdef _UNICODE
#define _tmainCRTStartup wmainCRTStartup
#else
#define _tmainCRTStartup mainCRTStartup
#endif

// TODO: This is horrible. You know it's horrible. I know it's horrible.
void _tmainCRTStartup() {
	_set_app_type(_crt_console_app);
	__NOTVCRUNTIME_init();
	atexit(__NOTVCRUNTIME_fini);
	int argc = 0;
	_TCHAR ** argv = NULL;
	exit(_tmain(argc, argv));
}
