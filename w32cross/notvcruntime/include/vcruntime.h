/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

#ifndef _MSC_VER
#error "You may need to pass -fms-compatibility-version, Clang isn't taking you seriously"
#endif

#include <stdint.h>
#include <stddef.h>

/* Stuff and things */
#include <crtdefs.h>
#include <vadefs.h>

/* Critically important. _Inout_ and all the rest live here. */
#include <sal.h>

#define _HAS_CXX17 0
