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
__declspec(allocate(".CRT$XIA")) _PIFV __xi_a[0];
__declspec(allocate(".CRT$XIZ")) _PIFV __xi_z[0];
/* C++ init */
__declspec(allocate(".CRT$XCA")) _PVFV __xc_a[0];
__declspec(allocate(".CRT$XCZ")) _PVFV __xc_z[0];
/* Pre-term */
__declspec(allocate(".CRT$XPA")) _PVFV __xp_a[0];
__declspec(allocate(".CRT$XPZ")) _PVFV __xp_z[0];
/* Final term */
__declspec(allocate(".CRT$XTA")) _PVFV __xt_a[0];
__declspec(allocate(".CRT$XTZ")) _PVFV __xt_z[0];

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
