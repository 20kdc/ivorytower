/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#pragma once

#include <vcruntime.h>
/* STL exception depends on this */
#include <corecrt_terminate.h>

extern "C++" {

/*
 * So I believe what's going on here is that exceptions are compared by *decorated class name*.
 * This is why it's so important type_info has the decorated name.
 */

namespace std {
	class exception {
	public:
		exception() : __message("unknown exception") {
		}
		exception(const char * msg) : __message(msg ? msg : "unknown exception") {
		}
		virtual ~exception() {}
		virtual const char * __CLR_OR_THIS_CALL what() const noexcept {
			return __message;
		}
	protected:
		const char * __message;
	};
	#define __NOTVCRUNTIME_EXC_OBVIOUS(base, ex) class ex : public base { public: ex() : base( #ex ) {} ex(const char * msg) : base(msg) {} };
	__NOTVCRUNTIME_EXC_OBVIOUS(exception, bad_alloc);
	__NOTVCRUNTIME_EXC_OBVIOUS(bad_alloc, bad_array_new_length);
	__NOTVCRUNTIME_EXC_OBVIOUS(exception, bad_exception);
}

}
