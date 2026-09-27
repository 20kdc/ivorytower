/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

// Hello and welcome to the amazing fake typeinfo!

#include <vcruntime_typeinfo.h>

type_info::~type_info() {
	// hi! I am a virtual destructor that shouldn't reasonably get called!
}
