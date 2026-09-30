/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

#include <pplinterface.h>

// This is so bad.
// By the guess of it, the 'right way' of doing this is as follows:
// https://github.com/MicrosoftDocs/cpp-docs/blob/main/docs/parallel/concrt/reference/currentscheduler-class.md#scheduletask
// ScheduleTask

namespace Concurrency {
	template<typename Result>
	class task {
	public:
		void wait() {
			// Technically, *this* call is permitted to block based on some dependency tracking which create_task cannot possibly perform.
			// However, it's probably better we eagerly execute.
			// If nothing else, task<void>'s layout is unclear.
			// From use, it looks somewhat like a unique_ptr/shared_ptr kind of deal.
		}
	};
	// Called-from: stl/include/future
	template<typename Lambda>
	task<void> create_task(Lambda l) {
		l();
		return task<void>();
	}
}
