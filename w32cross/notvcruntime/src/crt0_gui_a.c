/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "nvcr_con.h"

// TODO: This is horrible. You know it's horrible. I know it's horrible.
int WinMainCRTStartup() {
	return WinMain(NULL, NULL, NULL, 0);
}
