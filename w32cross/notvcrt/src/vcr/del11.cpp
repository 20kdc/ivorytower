/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#include "vcr_new.h"

void __cdecl operator delete(V_PAT) noexcept {
	::operator delete(p, a);
}
