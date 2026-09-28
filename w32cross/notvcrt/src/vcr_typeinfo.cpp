/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <corecrt_startup.h>
#include <vcruntime_typeinfo.h>

SLIST_HEADER __type_info_root_node = {};

void __type_info_root_node_destructor() {
	__std_type_info_destroy_list(&__type_info_root_node);
}

#pragma section(".CRT$XTY", read)
__declspec(allocate(".CRT$XTY")) static _PVFV __type_info_root_node_destructor_ptr = { __type_info_root_node_destructor };

// I'm not sure why, but Clang is really deathly afraid of emitting "??_7type_info@@6B@".
// Placing this here forces it to properly link to itself.
#pragma comment(linker, "/alternatename:??_7type_info@@6B@.1=??_7type_info@@6B@")

type_info::~type_info() {
	// hi! I am a virtual destructor that shouldn't reasonably get called!
	// This expression forces type_info's type_info and RTTI data to be emitted. We need this.
	(void) typeid(type_info);
}
