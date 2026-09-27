/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

#include <vcruntime.h>

// The symbols here are really, _really_ weird.
class type_info {
private:
	~type_info();
};

namespace std { using type_info = ::type_info; }
