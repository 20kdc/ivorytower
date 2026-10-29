/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#include <windows.h>

BYTE __tls_guard;

DWORD _tls_index = TLS_OUT_OF_INDEXES;
