/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#include "vcr_new.h"

void * __cdecl operator new(V_S) {
	// IMPORTANT: malloc() in MSVC always returns a valid freeable pointer if possible, no need to worry about malloc(0)
	void * data = malloc(s);
	if (!data)
		throw std::bad_alloc();
	return data;
}
