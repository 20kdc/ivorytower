/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

/*
 * Init/term sections.
 */

#include "crt0_common.h"

/*
 * THIS TABLE MUST BE INITIALIZED BY CRT0!
 * i.e. via _initialize_onexit_table(&onexit_local_table);
 * _initialize_onexit_table is clearly intended to be loaded by 1st thread.
 */
_onexit_table_t __NOTVCRUNTIME_onexit_table;

_onexit_t __cdecl _onexit(_onexit_t f) {
	return _register_onexit_function(&__NOTVCRUNTIME_onexit_table, f) ? f : 0;
}

/* Startup/shutdown for NOTVCRUNTIME. */
int __NOTVCRUNTIME_init() {
	_initialize_onexit_table(&__NOTVCRUNTIME_onexit_table);
	if (_initterm_e(__xi_a, __xi_z))
		return 0;
	_initterm(__xc_a, __xc_z);
	return 1;
}
void __NOTVCRUNTIME_fini() {
	_initterm(__xt_a, __xt_z);
}

/*
 * For apps (i.e. things that are not DLLs), we setup atexit here to use app CRT.
 * DLLs override this with a custom table set.
 */
#pragma comment(linker, "/alternatename:atexit=__NOTVCRUNTIME_ACRT_atexit")
#pragma comment(linker, "/alternatename:at_quick_exit=__NOTVCRUNTIME_ACRT_at_quick_exit")

void * __security_cookie = 0;

void __security_check_cookie(void * value) {
	// do nothing, we do not care.
}

/* We waste some bytes putting dummy values in here to prevent discard. */

/* C init */
#pragma section(".CRT$XIA", read)
__declspec(allocate(".CRT$XIA")) _PIFV __xi_a[1];
#pragma section(".CRT$XIZ", read)
__declspec(allocate(".CRT$XIZ")) _PIFV __xi_z[1];
/* C++ init */
#pragma section(".CRT$XCA", read)
__declspec(allocate(".CRT$XCA")) _PVFV __xc_a[1];
#pragma section(".CRT$XCZ", read)
__declspec(allocate(".CRT$XCZ")) _PVFV __xc_z[1];
/* Pre-term */
#pragma section(".CRT$XPA", read)
__declspec(allocate(".CRT$XPA")) _PVFV __xp_a[1];
#pragma section(".CRT$XPZ", read)
__declspec(allocate(".CRT$XPZ")) _PVFV __xp_z[1];
/* Final term */
#pragma section(".CRT$XTA", read)
__declspec(allocate(".CRT$XTA")) _PVFV __xt_a[1];
#pragma section(".CRT$XTZ", read)
__declspec(allocate(".CRT$XTZ")) _PVFV __xt_z[1];
