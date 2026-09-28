/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

#include <vcruntime.h>
#include <vcruntime_exception.h>

/*
 * Ok, so, here's a quick description of the metaphorical 'circle of life' for type_info,
 *  pieced together from things trying to replace everything OTHER than the CRT0.
 * You ready? Ok, here we go.
 *
 * The 'magic struct' filled in by the compiler is (here) the 'guts' struct.
 * Note there are no field name assumptions (not like Itanium ABI).
 *
 * Compiler puts decorated names into the executable. It does NOT put undecorated names.
 * So when you ask for an undecorated name, it's got to come from somewhere.
 * Specifically it gets allocated by `__std_type_info_name` (`vcruntime140`).
 * But if we're a DLL which might get unloaded, how does that name get freed?
 *
 * The answer is a little complicated.
 *
 * The CRT0 for each module holds `__type_info_root_node`.
 * This is an SLIST_HEADER with light wrapping for type management reasons.
 * We pass that in to `__std_type_info_name`. It handles the allocation, addition, etc.
 * During deinit, `__std_type_info_destroy_list` cleans up.
 */

extern "C++" {

// We pinkie-promise this will be defined (as an SLIST_HEADER wrapper) in vcr_typeinfo.cpp
class type_info;

extern union _SLIST_HEADER __type_info_root_node;

extern "C" {
	_CRTIMP2 int __std_type_info_destroy_list(union _SLIST_HEADER *);
	_CRTIMP2 int __std_type_info_compare(const void *, const void *);
	_CRTIMP2 size_t __std_type_info_hash(const void *);
	_CRTIMP2 const char * __std_type_info_name(void *, union _SLIST_HEADER *);
}

// The symbols here are really, _really_ weird.
class type_info {
public:
	virtual ~type_info();
	type_info(const type_info &) = delete;
	type_info& operator=(const type_info &) = delete;

	constexpr bool operator==(const type_info & rhs) const noexcept {
		return __std_type_info_compare(&guts, &rhs.guts) == 0;
	}
	constexpr bool operator!=(const type_info & rhs) const noexcept {
		return __std_type_info_compare(&guts, &rhs.guts) != 0;
	}
	bool before(const type_info & rhs) const noexcept {
		return __std_type_info_compare(&guts, &rhs.guts) < 0;
	}
	size_t hash_code() const noexcept {
		return __std_type_info_hash(&guts);
	}
	const char * name() const noexcept {
		return __std_type_info_name((void *) &guts, &__type_info_root_node);
	}
	const char * raw_name() const noexcept {
		return guts.dname;
	}
private:
	// This is the 'official start' of the class for some vcruntime functions.
	struct {
		// Undecorated name. Initialized to null. __std_type_info_name initializes.
		const char * udname;
		// Decorated name. Initialized by compiler, variable length.
		char dname[1];
	} guts;
};

namespace std {
	using type_info = ::type_info;
	__NOTVCRUNTIME_EXC_OBVIOUS(exception, bad_cast);
}

}
