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
#define _configure_t_argv _configure_wide_argv
#define __p___targv __p___wargv
#else
#define _tmainCRTStartup mainCRTStartup
#define _configure_t_argv _configure_narrow_argv
#define __p___targv __p___argv
#endif

void _tmainCRTStartup() {
	_set_app_type(_crt_console_app);
	__NOTVCRUNTIME_init();
	atexit(__NOTVCRUNTIME_fini);
	_configure_t_argv(_crt_argv_unexpanded_arguments);
	exit(_tmain(*__p___argc(), *__p___targv()));
}
