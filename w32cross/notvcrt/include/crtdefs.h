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

/* Import/Export flags */

/*
 * ucrtbase.dll/.lib
 * '_CRTIMP' is complicated. See ucrt/corecrt.h.
 * _ACRTIMP : Most functions go here.
 * _DCRTIMP : Feels like non-standard/weird functions use this, i.e. _setsystime.
 *            There's no meaningful pattern, though. If *anything* I'd maybe guess stripping from console configs?
 */
#ifndef CRTDLL
#define _CRTIMP __declspec(dllimport)
#else
#define _CRTIMP __declspec(dllexport)
#endif
/*
 * msvcp140.dll
 * CRTDLL2 is pretty clearly intended to indicate 'WE ARE THE STL'.
 */
#ifndef CRTDLL2
#define _CRTIMP2 __declspec(dllimport)
#else
#define _CRTIMP2 __declspec(dllexport)
#endif
/*
 * msvcp140.dll locale functions.
 * An interesting quandary occurs here, since MRTDLL indicates that these are for the *managed* runtime.
 * It's all here, though.
 */
#define _MRTIMP2 _CRTIMP2
/*
 * vcruntime140.dll (purecall handler, exposed by UCRT headers)
 * There's no VCRTDLL that I know of, and the whole point of this thing is that we *can't* just drop in VCRT.
 */
#define _VCRTIMP __declspec(dllimport)

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

#define _HAS_NODISCARD 1
#define _NODISCARD [[nodiscard]]

/* /GS support */

extern void * __security_cookie;
void __CRTDECL __security_init_cookie();
void __CRTDECL __security_check_cookie(void *);
