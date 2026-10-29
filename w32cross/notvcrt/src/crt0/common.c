/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

/*
 * Init/term sections.
 */

#include <stdlib.h>
#include "crt0_common.h"

/*
 * We waste some bytes putting dummy zero values in here to prevent discard.
 */

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
 * EXE:
 *  Destructors: _crt_atexit
 *  _onexit: Both _crt_atexit and _crt_at_quick_exit
 * DLL:
 *  Everything: exit_list
 */

typedef struct {
	SLIST_ENTRY entry;
	_PVFV fn;
} exitEntry_t;

/*
 * BEWARE!
 * This list is UNUSABLE in the EXE.
 * According to https://devblogs.microsoft.com/oldnewthing/20141017-00/?p=43823/ ,
 * the correct behaviour is 'CRT hired lackey'.
 */
static SLIST_HEADER dll_exit_list;
static int isDLL;

/*
 * For apps, they have the AppCRT atexit/at_quick_exit lists.
 * For DLLs, there is no obvious reason not to combine the lists.
 */
_onexit_t __cdecl _onexit(_onexit_t f) {
	if (!isDLL) {
		_crt_atexit((_PVFV) f);
		_crt_at_quick_exit((_PVFV) f);
		return f;
	}
	exitEntry_t * res = (exitEntry_t *) _aligned_malloc(sizeof(exitEntry_t), MEMORY_ALLOCATION_ALIGNMENT);
	res->entry.Next = NULL;
	res->fn = (_PVFV) f;
	if (!res)
		return 0;
	InterlockedPushEntrySList(&dll_exit_list, (PSLIST_ENTRY) res);
	return f;
}

int __cdecl atexit(_PVFV f) {
	if (isDLL) {
		return !!_onexit((_onexit_t) f);
	} else {
		return _crt_atexit(f);
	}
}

int __cdecl at_quick_exit(_PVFV f) {
	if (isDLL) {
		return !!_onexit((_onexit_t) f);
	} else {
		return _crt_at_quick_exit(f);
	}
}

int __NOTVCRUNTIME_exe_init() {
	/* yes, this is a stub, but one day it might not be. */
	__security_init_cookie();

	/*
	 * It may seem sensible to call __acrt_initialize.
	 * DON'T. It got called by appcrt_dllmain.cpp!
	 */
	isDLL = 0;

	/*
	 * Constructor logic is tested in test 'a_smoke'
	 * W32CROSS_REBUILDVCR=1 ./test.sh
	 */
	if (_initterm_e(__xi_a, __xi_z))
		return 0;
	_initterm(__xc_a, __xc_z);

	/*
	 * We're NOT a DLL; destructors are installed into _crt_atexit.
	 * initterm runs from A to Z.
	 * This means we should logically run destructors from Z to A.
	 * But atexit runs in reverse, so we do this by... iterating forward again!
	 * Isn't this fun?
	 */
	/* term first (so run last) */
	for (_PVFV * ptr = __xt_a; ptr != __xt_z; ptr++)
		if (*ptr)
			_crt_atexit(*ptr);
	/* then pre-term (so run before) */
	for (_PVFV * ptr = __xp_a; ptr != __xp_z; ptr++)
		if (*ptr)
			_crt_atexit(*ptr);

	return 1;
}

int __NOTVCRUNTIME_dll_init() {
	/* yes, this is a stub, but one day it might not be. */
	__security_init_cookie();

	isDLL = 1;
	InitializeSListHead(&dll_exit_list);

	/* As above. */
	if (_initterm_e(__xi_a, __xi_z))
		return 0;
	_initterm(__xc_a, __xc_z);

	/* Destructors are handled in the dll_fini function below. */
	return 1;
}

void __NOTVCRUNTIME_dll_fini() {
	while (1) {
		exitEntry_t * current = (exitEntry_t *) InterlockedPopEntrySList(&dll_exit_list);
		if (!current)
			break;
		current->fn();
		_aligned_free(current);
	}
	_initterm(__xp_a, __xp_z);
	_initterm(__xt_a, __xt_z);
}

/* -- LINKER ARG STUFFING AND RATIONALES THEREOF -- */

/*
 * interlocked etc.
 */
#pragma comment(linker, "/defaultlib:kernel32")

/*
 * hacks to use 'GNU' compiler-rt with 'MSVC' target
 * ugh... to be clear, there is a better way, BUT we can't use it because then Clang tries to link against its personal copy of compiler-rt (which it doesn't have)
 * 1. __chkstk
 *    Note: According to https://github.com/llvm/llvm-project/blob/fa5e5fcfc594f86297d0f3bfd3f15c37cfcb87c4/llvm/lib/Target/X86/X86FrameLowering.cpp#L744 :
 *    This function modifies SP on win32 (it's 'literally __alloca')
 *    On win64/arm64, it does NOT modify SP.
 *    If we were to use '__alloca' on win64, bad things would happen!
 */
#if defined(__i386__)
#pragma comment(linker, "/alternatename:__chkstk=__alloca")
#elif defined(__x86_64__)
#pragma comment(linker, "/alternatename:__chkstk=___chkstk_ms")
#endif

/*
 * Not included in general linker comment set. That might be a problem, mmm...
 */
#pragma comment(linker, "/merge:.CRT=.rdata")
