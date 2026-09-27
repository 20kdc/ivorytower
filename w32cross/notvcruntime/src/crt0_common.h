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
 */
int __NOTVCRUNTIME_init();
void __NOTVCRUNTIME_fini();

/*
 * This logic will link in the correct libvcruntime and ucrt.
 * It makes sense to link it in from here since:
 * A. crt0 is always used
 * B. libcmt/msvcrt is the only version-selected thing we control without further meddling
 */
#ifndef _DLL
	#ifndef _DEBUG
		#pragma comment(linker, "/defaultlib:libvcruntime.lib")
		#pragma comment(linker, "/defaultlib:libucrt.lib")
	#else
		#pragma comment(linker, "/defaultlib:libvcruntimed.lib")
		#pragma comment(linker, "/defaultlib:libucrtd.lib")
	#endif
#else
	#ifndef _DEBUG
		#pragma comment(linker, "/defaultlib:vcruntime.lib")
		#pragma comment(linker, "/defaultlib:ucrt.lib")
	#else
		#pragma comment(linker, "/defaultlib:vcruntimed.lib")
		#pragma comment(linker, "/defaultlib:ucrtd.lib")
	#endif
#endif

/*
 * Technically speaking, this might not be necessary for the DLL case.
 * However, it would be particularly asinine to filter it out.
 */
#pragma comment(linker, "/defaultlib:kernel32.lib")
