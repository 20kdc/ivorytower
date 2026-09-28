/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

#include <vcruntime.h>

_CRT_BEGIN_C_HEADER

/* This set can be found in vcruntime140. */

_VCRTIMP void* __cdecl memchr(const void * dest, int c, size_t n);
_VCRTIMP int __cdecl memcmp(const void * a, const void * b, size_t n);
_VCRTIMP void* __cdecl memcpy(void * dest, const void * src, size_t n);
_VCRTIMP void* __cdecl memmove(void * dest, const void * src, size_t n);
_VCRTIMP void* __cdecl memset(void * dest, int c, size_t n);

_VCRTIMP char * strchr(const char * s, int c);
_VCRTIMP char * strrchr(const char * s, int c);
_VCRTIMP char * strstr(const char * s, const char * c);

_VCRTIMP wchar_t * wcschr(const wchar_t * s, wchar_t c);
_VCRTIMP wchar_t * wcsrchr(const wchar_t * s, wchar_t c);
_VCRTIMP wchar_t * wcsstr(const wchar_t * s, const wchar_t * c);

_CRT_END_C_HEADER
