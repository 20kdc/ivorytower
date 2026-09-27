/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

/* C init */
__declspec(allocate(".CRT$XIA")) void * __xi_a[0];
__declspec(allocate(".CRT$XIZ")) void * __xi_z[0];
/* C++ init */
__declspec(allocate(".CRT$XCA")) void * __xc_a[0];
__declspec(allocate(".CRT$XCZ")) void * __xc_z[0];
/* Pre-term */
__declspec(allocate(".CRT$XPA")) void * __xp_a[0];
__declspec(allocate(".CRT$XPZ")) void * __xp_z[0];
/* Final term */
__declspec(allocate(".CRT$XTA")) void * __xt_a[0];
__declspec(allocate(".CRT$XTZ")) void * __xt_z[0];
