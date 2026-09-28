/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

/*
 * Clang does some weird things here.
 * It *has* a vadefs.h that's supposed to override this one.
 * As it is, we:
 * 1. include stdarg.h to get Clang's va_list
 * 2. Define dummies so that Clang can override them (since Clang won't override them if we don't define them).
 * We also give them definitions which aren't technically Clang's definitions but should still work.
 * Y'know, as a security measure...
 */

#include <stdarg.h>

#define _crt_va_start __builtin_va_start
#define _crt_va_end __builtin_va_end
#define _crt_va_arg __builtin_va_arg

#define __crt_va_start __builtin_va_start
#define __crt_va_end __builtin_va_end
#define __crt_va_arg __builtin_va_arg
