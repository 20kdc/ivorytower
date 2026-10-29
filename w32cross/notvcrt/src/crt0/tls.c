/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#include "crt0_common.h"

#include <stdlib.h>

/* -- TLS C++ constructors -- */

/* Thread init */
#pragma section(".CRT$XDA", read)
_CRTALLOC(".CRT$XDA") _PVFV __xd_a[1];
#pragma section(".CRT$XDZ", read)
_CRTALLOC(".CRT$XDZ") _PVFV __xd_z[1];

/*
 * LLVM only /include: s this without further elaboration.
 * Note this is included in __xl_a.
 */
void __stdcall __dyn_tls_init(void * a, unsigned long reason, void * reserved) {
	if (reason == DLL_PROCESS_ATTACH || reason == DLL_THREAD_ATTACH)
		_initterm(__xd_a, __xd_z);
}

/* -- TLS C++ destructors -- */

static __declspec(thread) _PVFV * __tlregdtor_list;
static __declspec(thread) size_t __tlregdtor_count;

/*
 * Called by TLS initializers.
 * Notably, this means it's called per-thread.
 * So we're being expected to maintain a per-thread TLS dtor list now.
 */
int __tlregdtor(_PVFV f) {
	if (!__tlregdtor_list) {
		__tlregdtor_list = malloc(sizeof(_PVFV));
		__tlregdtor_list[0] = f;
		__tlregdtor_count = 1;
		return 0;
	} else {
		__tlregdtor_list = realloc(__tlregdtor_list, sizeof(_PVFV) * (__tlregdtor_count + 1));
		__tlregdtor_list[__tlregdtor_count++] = f;
		return 0;
	}
}

static void __stdcall __dyn_tls_fini(void * a, unsigned long reason, void * reserved) {
	if (reason == DLL_PROCESS_DETACH || reason == DLL_THREAD_DETACH) {
		while (__tlregdtor_count > 0) {
			_PVFV fn = __tlregdtor_list[--__tlregdtor_count];
			fn();
		}
	}
}

/* -- TLS callback linkage -- */

/*
 * Alright, so, things get a bit awkward here.
 *
 * The main problems to handle are:
 * 1. The TLS callback list shouldn't be compiled in unless we use TLS.
 * 2. __xl_z gets dropped because of lack of references.
 *
 * To solve all three problems at once:
 * __NOTVCRUNTIME_xl_a_impl is __xl_exttls followed by __xl_a.
 * Then some assembly trickery is used to alias __xl_a from that.
 * This means that _tls_used indirectly includes __xl_z and all is well.
 * The exttls_t can also be used for auxiliary data we want the CRT to be able to steal.
 * ...for example, the TLS emulator.
 */

extern exttls_t __xl_exttls;

/*
 * TLS callbacks.
 * Notably, this is a zero-terminated list.
 * In theory this would mean you need to start at __xl_a + 1.
 * However, there's a perfectly legitimate values we can put in to fill the gap.
 * However-however, we also use this to hide the exttls pointer.
 */
#pragma section(".CRT$XLA", read)
_CRTALLOC(".CRT$XLA") PIMAGE_TLS_CALLBACK __NOTVCRUNTIME_xl_a_impl[] = {
	(PIMAGE_TLS_CALLBACK) (void *) &__xl_exttls,
	/* __xl_a goes here */
	__dyn_tls_init
};
extern PIMAGE_TLS_CALLBACK __xl_a[];

#if defined(__i386__)
__asm__(
	".global ___xl_a\n"
	"___xl_a = ___NOTVCRUNTIME_xl_a_impl + 4\n"
);
#elif !defined(_WIN64)
__asm__(
	".global __xl_a\n"
	"__xl_a = __NOTVCRUNTIME_xl_a_impl + 4\n"
);
#else
__asm__(
	".global __xl_a\n"
	"__xl_a = __NOTVCRUNTIME_xl_a_impl + 8\n"
);
#endif

#pragma section(".CRT$XLZ", read)
_CRTALLOC(".CRT$XLZ") PIMAGE_TLS_CALLBACK __xl_z[] = {
	__dyn_tls_fini,
	NULL
};

/* -- TLS state -- */

#if defined(__i386__)
__asm__(
	".global __tls_array\n"
	"__tls_array = 0x2C\n"
);
#endif

BYTE __tls_guard;

/* -- Directories -- */

DWORD _tls_index = TLS_OUT_OF_INDEXES;

/*
 * declspec_thread.s puts into ".tls$"
 */
_CRTALLOC(".tls") char _tls_start[1];
_CRTALLOC(".tls$ZZZ") char _tls_end[1];

_CRTALLOC(".rdata$T") IMAGE_TLS_DIRECTORY _tls_used = {
#ifdef _WIN64
	.StartAddressOfRawData = (ULONGLONG) _tls_start,
	.EndAddressOfRawData = (ULONGLONG) _tls_end,
	.AddressOfIndex = (ULONGLONG) &_tls_index,
	.AddressOfCallBacks = (ULONGLONG) __xl_a
#else
	.StartAddressOfRawData = (DWORD) _tls_start,
	.EndAddressOfRawData = (DWORD) _tls_end,
	.AddressOfIndex = (DWORD) &_tls_index,
	.AddressOfCallBacks = (DWORD) __xl_a
#endif
};

static exttls_t __xl_exttls = {
	.xl_z = __xl_z
};
