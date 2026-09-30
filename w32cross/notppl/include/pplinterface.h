/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

#include <vcruntime.h>

#pragma comment(linker, "/defaultlib:concrt140.lib")

// Given the pplinterface vs. ppltasks divide, it's pretty clear this contains the low-level implementation.
// Meanwhile ppltasks contains the high-level 'STLish frontend'.

// #define _PPLIMP __declspec(dllimport) maybe?

// Interface can be gotten from i.e.
// https://github.com/MicrosoftDocs/cpp-docs/blob/f2355df9f7136d8a2097193fc507882a7caeb5f5/docs/parallel/concrt/reference/concurrency-namespace.md#typedefs
// https://github.com/MicrosoftDocs/cpp-docs/blob/main/docs/parallel/concrt/reference/currentscheduler-class.md#scheduletask
// and of course the defs files

// not sure why this has a capital C, but this matches STL use and defs
// possible using concurrency = Concurrency; ? but don't want to encourage this...
namespace Concurrency {
	class Context {
		static __cdecl void _SpinYield();
	};
	typedef void (__cdecl * TaskProc)(void *);
	class CurrentScheduler __declspec(dllimport) {
	public:
		static void __cdecl ScheduleTask(TaskProc, void *);
	};
}
