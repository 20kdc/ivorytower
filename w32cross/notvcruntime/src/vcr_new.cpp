/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#include <stdlib.h>
#include <vcruntime_new.h>
#include <corecrt_malloc.h>

std::nothrow_t nothrow;

/*
 * Important notes:
 * 1. malloc can be overridden by user. Is this intended?
 * 2. _malloc_base (and also default malloc) will set errno. Is this intended?
 *    It will also do callnewh for us, so DO NOT DO THAT.
 * 3. We are very likely supposed to call _Xbad_alloc to throw an std::bad_alloc exception.
 */

[[noreturn]] _CRTIMP2 void __CRTDECL _Xbad_alloc();

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

/* -- Unaligned base operations -- */

void * __cdecl operator new(V_S) {
	// IMPORTANT: malloc() in MSVC always returns a valid freeable pointer if possible, no need to worry about malloc(0)
	void * data = malloc(s);
	if (!data)
		_Xbad_alloc();
	return data;
}
void * __cdecl operator new(V_ST) noexcept {
	// Apparently this has to be done for overridability reasons. >:(
	try { return ::operator new(s); } catch (...) { return nullptr; }
}
void __cdecl operator delete(V_P) noexcept {
	free(p);
}

/* -- Aligned base operations -- */

void * __cdecl operator new(V_SA) {
	void * data = _aligned_malloc(s, a);
	if (!data)
		_Xbad_alloc();
	return data;
}
void * __cdecl operator new(V_SAT) noexcept {
	try { return ::operator new(s, a); } catch (...) { return nullptr; }
}
void __cdecl operator delete(V_PA) noexcept {
	_aligned_free(p);
}

/* -- Array-to-object proxy operations -- */

void * __cdecl operator new[](V_S) {
	return ::operator new(s);
}
void __cdecl operator delete[](V_P) noexcept {
	::operator delete(p);
}

/* -- Proxy operations -- */

// TODO ARRAYS SHOULD PROXY TO ARRAYS

void * __cdecl operator new[](V_ST) noexcept {
	return ::operator new(s, t);
}
void * __cdecl operator new[](V_SA) {
	return ::operator new(s, a);
}
void * __cdecl operator new[](V_SAT) noexcept {
	return ::operator new(s, a, t);
}

void __cdecl operator delete(V_PT) noexcept {
	::operator delete(p);
}
void __cdecl operator delete(V_PS) noexcept {
	::operator delete(p);
}
void __cdecl operator delete(V_PAT) noexcept {
	::operator delete(p);
}
void __cdecl operator delete(V_PSA) noexcept {
	::operator delete(p);
}
void __cdecl operator delete[](V_PT) noexcept {
	::operator delete(p);
}
void __cdecl operator delete[](V_PS) noexcept {
	::operator delete(p);
}
void __cdecl operator delete[](V_PA) noexcept {
	::operator delete(p);
}
void __cdecl operator delete[](V_PAT) noexcept {
	::operator delete(p);
}
void __cdecl operator delete[](V_PSA) noexcept {
	::operator delete(p);
}
