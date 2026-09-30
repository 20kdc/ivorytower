/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

#include <process.h>

void * __security_cookie = 0;

/*
 * BEWARE: Our current crt0 DOES NOT CALL THIS FUNCTION.
 */
void __stdcall __security_init_cookie() {
	/* we retain our general attitude of not caring about this. */
}

void __stdcall __security_check_cookie(void * value) {
	/* do nothing, we do not care. */
}
