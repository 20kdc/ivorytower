/*
 * This is only valid for the 'app CRT'.
 */

#include <corecrt_startup.h>

int __cdecl __NOTVCRUNTIME_ACRT_atexit(void (__cdecl * callback)()) {
	return _crt_atexit(callback);
}

int __cdecl __NOTVCRUNTIME_ACRT_at_quick_exit(void (__cdecl * callback)()) {
	return _crt_at_quick_exit(callback);
}
