/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#include <windows.h>
#include <corecrt_startup.h>

#if defined(__i386__)
__asm__(
	".global __tls_array\n"
    "__tls_array = 0x2C\n"
);
#endif

BYTE __tls_guard;

DWORD _tls_index = TLS_OUT_OF_INDEXES;

__declspec(allocate(".tls")) char _tls_start[1];
__declspec(allocate(".tls$ZZZ")) char _tls_end[1];

__declspec(allocate(".rdata$T")) IMAGE_TLS_DIRECTORY _tls_used = {
#ifdef _WIN64
	.StartAddressOfRawData = (ULONGLONG) _tls_start,
	.EndAddressOfRawData = (ULONGLONG) _tls_end,
	.AddressOfIndex = (ULONGLONG) &_tls_index
#else
	.StartAddressOfRawData = (DWORD) _tls_start,
	.EndAddressOfRawData = (DWORD) _tls_end,
	.AddressOfIndex = (DWORD) &_tls_index
#endif
};
