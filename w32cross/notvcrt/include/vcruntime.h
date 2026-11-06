/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

#ifndef _MSC_VER
#error "You may need to pass -fms-compatibility-version, Clang isn't taking you seriously"
#endif

/* MinGW compatibility. This must be handled as early as possible. { */
#define __MINGW_EXTENSION
#define __MINGW_ATTRIB_NORETURN __declspec(noreturn)
/* For excpt.h __try1 */
#define __MINGW64_STRINGIFY_2(x) #x
#define __MINGW64_STRINGIFY(x) __MINGW64_STRINGIFY_2(x)
#ifdef __i386__
#define __MINGW_USYMBOL_2(x) _ ## x
#define __MINGW_USYMBOL(x) __MINGW_USYMBOL_2(x)
#else
#define __MINGW_USYMBOL(x) x
#endif
/* } */

/*
 * Other included things.
 * Be very careful with this list; most things you 'could' add causes circularity.
 * The general rule you should follow for inclusion is,
 * 'Does deleting this break w10/4vcruntime.sh ?'
 */

#include <stdint.h>

/*
 * Not 'critical', but dependencyless and generally wise to include.
 * Note that upstream UCRT stddef.h ends up including this file.
 * So maybe not that wise. Should review at some point
 */
#include <stddef.h>

/* Intentionally circular for now. More thought on this is necessary. */
#include <crtdefs.h>

/* Same kinda reasoning as stddef. */
#include <vadefs.h>

/*
 * Critically important. _Inout_ and all the rest live here.
 * If this is not on-path for CRT functions things go VERY wrong.
 */
#include <sal.h>

/* Feature flags. */

#define _HAS_CXX17 0
#define _HAS_EXCEPTIONS 1
#define _HAS_UNEXPECTED 1
#define _HAS_NODISCARD 1
#define _NODISCARD [[nodiscard]]

/* C++ guards */

#ifdef __cplusplus
#define _CRT_BEGIN_C_HEADER extern "C" {
#define _CRT_END_C_HEADER }
#else
#define _CRT_BEGIN_C_HEADER
#define _CRT_END_C_HEADER
#endif

/* Calling conventions, etc. */

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

/* /GS support */

extern void * __security_cookie;
void  __security_init_cookie();
void __fastcall __security_check_cookie(void *);
