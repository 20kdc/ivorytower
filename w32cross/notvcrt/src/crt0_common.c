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
#include "corecrt_startup.h"
#include "crtdefs.h"

/* We waste some bytes putting dummy values in here to prevent discard. */

#pragma comment(linker, "/merge:.CRT=.rdata")

#define _CRTALLOC(s) __declspec(allocate(s))

/* C init */
#pragma section(".CRT$XIA", read)
_CRTALLOC(".CRT$XIA") _PIFV __xi_a[1];
#pragma section(".CRT$XIZ", read)
_CRTALLOC(".CRT$XIZ") _PIFV __xi_z[1];
/* C++ init */
#pragma section(".CRT$XCA", read)
_CRTALLOC(".CRT$XCA") _PVFV __xc_a[1];
#pragma section(".CRT$XCZ", read)
_CRTALLOC(".CRT$XCZ") _PVFV __xc_z[1];

/* Pre-term */
#pragma section(".CRT$XPA", read)
_CRTALLOC(".CRT$XPA") _PVFV __xp_a[1];
#pragma section(".CRT$XPZ", read)
_CRTALLOC(".CRT$XPZ") _PVFV __xp_z[1];
/* Final term */
#pragma section(".CRT$XTA", read)
_CRTALLOC(".CRT$XTA") _PVFV __xt_a[1];
#pragma section(".CRT$XTZ", read)
_CRTALLOC(".CRT$XTZ") _PVFV __xt_z[1];

/*
 * Rather than try to figure out how to make sure we don't get a symbol conflict,
 *  we include all three tables even though we 'shouldn't'.
 */
static _onexit_table_t atexit_table, at_quick_exit_table, onexit_table;

static int isDLL;

int __cdecl atexit(void (__cdecl * f)()) {
	if (!isDLL)
		return _register_onexit_function(&atexit_table, (_onexit_t) f);
	return _crt_atexit(f);
}

int __cdecl at_quick_exit(void (__cdecl * f)()) {
	if (!isDLL)
		return _register_onexit_function(&at_quick_exit_table, (_onexit_t) f);
	return _crt_at_quick_exit(f);
}

_onexit_t __cdecl _onexit(_onexit_t f) {
	return _register_onexit_function(&onexit_table, f) ? f : 0;
}

int __NOTVCRUNTIME_init(int isDLLV) {
	isDLL = isDLLV;
	if (isDLL) {
		_initialize_onexit_table(&atexit_table);
		_initialize_onexit_table(&at_quick_exit_table);
	}
	_initialize_onexit_table(&onexit_table);
	if (_initterm_e(__xi_a, __xi_z))
		return 0;
	_initterm(__xc_a, __xc_z);
	return 1;
}
void __NOTVCRUNTIME_fini() {
	if (isDLL) {
		_execute_onexit_table(&atexit_table);
		_execute_onexit_table(&at_quick_exit_table);
	}
	_initterm(__xt_a, __xt_z);
}
