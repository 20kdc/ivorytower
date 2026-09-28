/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

#include <vcruntime.h>

_CRT_BEGIN_C_HEADER

/*
 * MinGW says this lives in corecrt_startup.h; while that'd make sense seeing as it's an API exclusive to corecrt_startup, it doesn't.
 * ucrt/corecrt_startup.h shows that file pulls in us and we have it instead.
 */
typedef enum _crt_argv_mode {
	_crt_argv_no_arguments,
	_crt_argv_unexpanded_arguments,
	_crt_argv_expanded_arguments
} _crt_argv_mode;

_CRT_END_C_HEADER
