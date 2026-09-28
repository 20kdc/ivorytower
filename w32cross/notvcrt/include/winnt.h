/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

/*
 * Processor architectures.
 * Theoretically, Windows.h should be doing this, but STL indicates that it isn't, or something else is wrong. IDK.
 */
#if defined(_M_X64)
#ifndef _AMD64_
#define _AMD64_
#endif
#elif defined(_M_IX86)
#ifndef _X86_
#define _X86_
#endif
#elif defined(_M_ARM64)
#ifndef _ARM64_
#define _ARM64_
#endif
#else
#error "crtdefs.h"
#endif

#include_next <winnt.h>
