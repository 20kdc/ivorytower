/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

#include <vcruntime.h>
#include <vcruntime_exception.h>

extern "C++" {

namespace std {
	enum class align_val_t : size_t {};
	struct nothrow_t { explicit nothrow_t() = default; };
	extern const ::std::nothrow_t nothrow;
}

/* -- Unaligned base operations -- */

void * __cdecl operator new(size_t);
void * __cdecl operator new(size_t, const ::std::nothrow_t &) noexcept;
void __cdecl operator delete(void *) noexcept;

/* -- Aligned base operations -- */

void * __cdecl operator new(size_t, ::std::align_val_t);
void * __cdecl operator new(size_t, ::std::align_val_t, const ::std::nothrow_t &) noexcept;
void __cdecl operator delete(void *, ::std::align_val_t) noexcept;

/* -- Array-to-object proxy operations -- */

void * __cdecl operator new[](size_t);
void __cdecl operator delete[](void *) noexcept;

/* -- Proxy operations -- */

void * __cdecl operator new[](size_t, const ::std::nothrow_t &) noexcept;
void * __cdecl operator new[](size_t, ::std::align_val_t);
void * __cdecl operator new[](size_t, ::std::align_val_t, const ::std::nothrow_t &) noexcept;

void __cdecl operator delete(void *, const ::std::nothrow_t &) noexcept;
void __cdecl operator delete(void *, size_t) noexcept;
void __cdecl operator delete(void *, ::std::align_val_t, const ::std::nothrow_t &) noexcept;
void __cdecl operator delete(void *, size_t, ::std::align_val_t) noexcept;
void __cdecl operator delete[](void *, const ::std::nothrow_t &) noexcept;
void __cdecl operator delete[](void *, size_t) noexcept;
void __cdecl operator delete[](void *, ::std::align_val_t) noexcept;
void __cdecl operator delete[](void *, ::std::align_val_t, const ::std::nothrow_t &) noexcept;
void __cdecl operator delete[](void *, size_t, ::std::align_val_t) noexcept;

/* -- Inline proxy operations -- */

inline void * __cdecl operator new(size_t s, void * ptr) noexcept { return ptr; }
inline void * __cdecl operator new[](size_t s, void * ptr) noexcept { return ptr; }
inline void __cdecl operator delete(void * x, void * y) noexcept {}
inline void __cdecl operator delete[](void * x, void * y) noexcept {}

}
