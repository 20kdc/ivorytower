/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

/*
 * Contains stuff common to all CRT0 files.
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <corecrt_startup.h>

#ifdef HAS_DEBUG_PUTS
#warning Debug puts enabled do not commit
#include <stdio.h>
#define DEBUGF(...) fprintf(stderr, __VA_ARGS__)
#else
#define DEBUGF(...)
#endif

#define _CRTALLOC(s) __declspec(allocate(s))

/* C init */
extern _PIFV __xi_a[1];
extern _PIFV __xi_z[1];
/* C++ init */
extern _PVFV __xc_a[1];
extern _PVFV __xc_z[1];

/* Pre-term */
extern _PVFV __xp_a[1];
extern _PVFV __xp_z[1];
/* Final term */
extern _PVFV __xt_a[1];
extern _PVFV __xt_z[1];

/*
 * crt0 init/finalizer functions.
 * 0 return in __NOTVCRUNTIME_*_init indicates mysterious failure.
 * Be warned: These functions *MUST* only be called once ever per module, each.
 */
int __NOTVCRUNTIME_dll_init();
int __NOTVCRUNTIME_exe_init();

/*
 * This only runs for DLLs.
 * EXEs setup everything this *would* do with _crt_atexit in advance.
 * This is based on logic implied by https://devblogs.microsoft.com/oldnewthing/20141017-00/?p=43823/
 */
void __NOTVCRUNTIME_dll_fini();

/* TLS emulator */
void __stdcall __NOTVCRUNTIME_tlsemu(HMODULE * module, int reason, void * reserved);

/*
 * A pointer to this lives directly before __xl_a if-and-only-if TLS is in use.
 * This is:
 * 1. So that LLVM doesn't drop the (rather important) symbols
 * 2.
 */
typedef struct {
	/* __xl_z entry */
	PIMAGE_TLS_CALLBACK * xl_z;
} exttls_t;
