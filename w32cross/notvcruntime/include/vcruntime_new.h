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

/*
 * In order to implement this mess correctly, the numbered variants for the operators on cppreference are best.
 * https://en.cppreference.com/cpp/memory/new/operator_new
 * https://en.cppreference.com/cpp/memory/new/operator_delete
 */

/* new: replacable allocation functions (1-4) */

void * __cdecl operator new(size_t);
void * __cdecl operator new[](size_t);
void * __cdecl operator new(size_t, ::std::align_val_t);
void * __cdecl operator new[](size_t, ::std::align_val_t);

/* new: replacable non-throwing allocation functions (5-8) */

void * __cdecl operator new(size_t, const ::std::nothrow_t &) noexcept;
void * __cdecl operator new[](size_t, const ::std::nothrow_t &) noexcept;
void * __cdecl operator new(size_t, ::std::align_val_t, const ::std::nothrow_t &) noexcept;
void * __cdecl operator new[](size_t, ::std::align_val_t, const ::std::nothrow_t &) noexcept;

/*
 * delete: replacable usual deallocation functions (1-8)
 * cppref says (2,4) call (1,3)
 */

void __cdecl operator delete(void *) noexcept;
void __cdecl operator delete[](void *) noexcept;
void __cdecl operator delete(void *, ::std::align_val_t) noexcept;
void __cdecl operator delete[](void *, ::std::align_val_t) noexcept;

/* cppref says (5-8) call (1-4) */

void __cdecl operator delete(void *, size_t) noexcept;
void __cdecl operator delete[](void *, size_t) noexcept;
void __cdecl operator delete(void *, size_t, ::std::align_val_t) noexcept;
void __cdecl operator delete[](void *, size_t, ::std::align_val_t) noexcept;

/*
 * delete: replacable placement deallocation functions (9-12)
 * cppref says (9,10) call (1,2) ('global replacements')
 * nothing is said of (11,12) but elsewhere 'same as 9,10', can be assumed they call (3,4)
 */

void __cdecl operator delete(void *, const ::std::nothrow_t &) noexcept;
void __cdecl operator delete[](void *, const ::std::nothrow_t &) noexcept;
void __cdecl operator delete(void *, ::std::align_val_t, const ::std::nothrow_t &) noexcept;
void __cdecl operator delete[](void *, ::std::align_val_t, const ::std::nothrow_t &) noexcept;

/* -- 'Placement' (new 9-10, delete 13-14) -- */

inline void * __cdecl operator new(size_t s, void * ptr) noexcept { return ptr; }
inline void * __cdecl operator new[](size_t s, void * ptr) noexcept { return ptr; }
inline void __cdecl operator delete(void * x, void * y) noexcept {}
inline void __cdecl operator delete[](void * x, void * y) noexcept {}

}
