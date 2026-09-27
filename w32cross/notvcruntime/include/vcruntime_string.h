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

_ACRTIMP void* __cdecl memchr(const void * dest, int c, size_t n);
_ACRTIMP int __cdecl memcmp(void * dest, const void * src, size_t n);
_ACRTIMP void* __cdecl memcpy(void * dest, const void * src, size_t n);
_ACRTIMP void* __cdecl memmove(void * dest, const void * src, size_t n);
_ACRTIMP void* __cdecl memset(void * dest, int c, size_t n);

_ACRTIMP char * strchr(const char * s, int c);
_ACRTIMP char * strrchr(const char * s, int c);
_ACRTIMP char * strstr(const char * s, const char * c);

_ACRTIMP wchar_t * wcschr(const wchar_t * s, wchar_t c);
_ACRTIMP wchar_t * wcsrchr(const wchar_t * s, wchar_t c);
_ACRTIMP wchar_t * wcsstr(const wchar_t * s, const wchar_t * c);

_CRT_END_C_HEADER
