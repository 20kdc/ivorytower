/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

void * __security_cookie = 0;

void __security_check_cookie(void * value) {
	// do nothing, we do not care.
}
