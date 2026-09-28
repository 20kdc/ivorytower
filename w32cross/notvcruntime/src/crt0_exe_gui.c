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

#ifdef _UNICODE
#define _tWinMainCRTStartup wWinMainCRTStartup
#define _get_t_winmain_command_line _get_wide_winmain_command_line
#else
#define _tWinMainCRTStartup WinMainCRTStartup
#define _get_t_winmain_command_line _get_narrow_winmain_command_line
#endif

// TODO: This is horrible. You know it's horrible. I know it's horrible.
void _tWinMainCRTStartup() {
	_set_app_type(_crt_gui_app);
	__NOTVCRUNTIME_init();
	atexit(__NOTVCRUNTIME_fini);
	_TCHAR * cmdline = _get_t_winmain_command_line();
	exit(_tWinMain(NULL, NULL, cmdline, 0));
}
