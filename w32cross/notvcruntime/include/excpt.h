/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

#define _CRTIMP __declspec(dllimport)
#define _VCRTIMP _CRTIMP

typedef enum _EXCEPTION_DISPOSITION {
	ExceptionContinueExecution = 0,
	ExceptionContinueSearch = 1,
	ExceptionNestedException = 2,
	ExceptionCollidedUnwind = 3
} EXCEPTION_DISPOSITION;
