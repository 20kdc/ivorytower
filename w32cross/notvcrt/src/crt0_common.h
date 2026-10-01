/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

/*
 * Contains stuff common to all CRT0 files.
 */

#include <corecrt_startup.h>

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
 * 0 return in __NOTVCRUNTIME_init indicates mysterious failure.
 * Be warned: These functions *MUST* only be called once ever per module, each.
 */
int __NOTVCRUNTIME_init(int isDLL);

/*
 * This only runs for DLLs.
 * EXEs setup everything this *would* do with _crt_atexit in advance.
 * This is based on logic implied by https://devblogs.microsoft.com/oldnewthing/20141017-00/?p=43823/
 */
void __NOTVCRUNTIME_dll_fini();

#pragma comment(linker, "/defaultlib:kernel32")
