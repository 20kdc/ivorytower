/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

/*
 * Circular, I know.
 * I'm putting things where they 'ought to go' but hedge bets by including vcruntime.h
 */
#include <vcruntime.h>

/* C++ guards */

#ifdef __cplusplus
#define _CRT_BEGIN_C_HEADER extern "C" {
#define _CRT_END_C_HEADER }
#else
#define _CRT_BEGIN_C_HEADER
#define _CRT_END_C_HEADER
#endif

/* Imports */

#define _CRTIMP __declspec(dllimport)
#define _CRTIMP2 _CRTIMP
#define _VCRTIMP _CRTIMP
#define _MRTIMP2 _CRTIMP

/* Calling conventions */

#define __CLR_OR_THIS_CALL __thiscall
#define __CLRCALL_PURE_OR_CDECL __cdecl
#define __CLRCALL_OR_CDECL __cdecl

#define __CRTDECL __cdecl

/* Deprecation notices */

#define _CRT_INSECURE_DEPRECATE(f)
#define _CRT_INSECURE_DEPRECATE_MEMORY(lies)
#define _CRT_INSECURE_DEPRECATE_GLOBALS(bad)

#define _CRT_DEPRECATE_TEXT(p)
#define _CRT_SATELLITE_CODECVT_IDS_NOIMPORT

/* Markers */

#define _NODISCARD
#define _CRTALLOC(s) __declspec(allocate(s))

/* /GS support */

extern void * __security_cookie;
void __CRTDECL __security_init_cookie();
void __CRTDECL __security_check_cookie(void *);
