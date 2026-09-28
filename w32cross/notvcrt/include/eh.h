/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

#include <vcruntime.h>

/* https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/uncaught-exception?view=msvc-140 */

_CRT_BEGIN_C_HEADER

_VCRTIMP bool __uncaught_exception();
_VCRTIMP bool __uncaught_exceptions();

_CRT_END_C_HEADER
