/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
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
