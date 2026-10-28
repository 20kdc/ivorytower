/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

/*
 * We need to split up new so that user-defined allocation functions work.
 * This really sucks, but it is what it is.
 */

#include <stdlib.h>
#include <vcruntime_new.h>
#include <corecrt_malloc.h>

/*
 * Important notes:
 * 1. malloc can be overridden by user. Is this intended?
 * 2. _malloc_base (and also default malloc) will set errno. Is this intended?
 *    It will also do callnewh for us, so DO NOT DO THAT.
 * 3. We are very likely supposed to call _Xbad_alloc to throw an std::bad_alloc exception.
 */

#define V_S size_t s
#define V_ST size_t s, const std::nothrow_t & t
#define V_P void * p
#define V_SA size_t s, std::align_val_t a
#define V_SAT size_t s, std::align_val_t a, const std::nothrow_t & t
#define V_PA void * p, std::align_val_t a
#define V_PT void * p, const std::nothrow_t & t
#define V_PS void * p, size_t s
#define V_PAT void * p, std::align_val_t a, const std::nothrow_t & t
#define V_PSA void * p, size_t s, std::align_val_t a

#define NEWPROX(v) try { return v; } catch (...) { return nullptr; }
