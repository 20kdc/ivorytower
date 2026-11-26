/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#include <windows.h>

/*
 * See static_constructors.cpp!
 * TSS aka GuardVar is a true global initialized to 0.
 */

/*
 * Not sure how this is to be manipulated.
 * I'm not sure what it solves, so leave it alone.
 * Instead, we use TSS as a spinlock.
 */
__declspec(thread) int _Init_thread_epoch = -2;

void _Init_thread_header(volatile LONG * tss) {
	while (1) {
		LONG v = InterlockedCompareExchange(tss, -1, 0);
		/* v is previous value */
		if (v == 0) {
			/* 0: locked by us */
			return;
		} else if (v == -1) {
			/* -1: in progress, wait */
		} else {
			/* -2: completed */
			return;
		}
	}
}

void _Init_thread_footer(volatile LONG * tss) {
	InterlockedCompareExchange(tss, -2, -1);
}

void _Init_thread_abort(volatile LONG * tss) {
	InterlockedCompareExchange(tss, 0, -1);
}
