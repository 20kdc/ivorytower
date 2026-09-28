/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

#include <vcruntime.h>

#ifdef __i386__
typedef int jmp_buf[16];
#elif defined(__x86_64__)
typedef __declspec(align(16)) struct { char x[256]; } ____setjmp_opaque_amd64;
typedef ____setjmp_opaque_amd64 jmp_buf[1];
#elif defined(__aarch64__)
typedef __int64 jmp_buf[24];
#else
#error "Target architecture not supported in NOTVCRUNTIME setjmp.h"
#endif
