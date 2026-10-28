/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

/*
 * We don't actually care about /gs from a security perspective.
 * However, it's really useful for debugging ABI issues on x86.
 * For that reason, we implement a 'watered down' version there specifically.
 * Notably, pretty much this entire file has to be carefully prodded assembly.
 * It might be better if this was a .S file.
 */

__asm__(
	".global __security_cookie\n"
	".global ___security_cookie\n"
	".data\n"
#ifdef __i386__
	".align 4\n"
#else
	".align 8\n"
#endif
	"__security_cookie:\n"
	"___security_cookie:\n"
#ifdef __i386__
	".byte 0x4a, 0x7f, 0x96, 0xe7\n"
#else
	".byte 0x4a, 0x7f, 0x96, 0xe7, 0x5d, 0xdb, 0x65, 0x7b\n"
#endif
	".text\n"
	".global __security_check_cookie\n"
	".global @__security_check_cookie@4\n"
	"__security_check_cookie:\n"
	"@__security_check_cookie@4:\n"
#ifdef __i386__
	"cmpl %ecx, __security_cookie\n"
	"jne .OOPS\n"
	"ret\n"
	".OOPS:\n"
	/*
	 * Supposedly this might not be available pre-Windows 8.
	 * However, if you reach this codepath, you're already well into UB.
	 * If you are intentionally triggering GS failures and relying on their
	 *  consequences being precisely defined, you're screwing up.
	 */
	"movl %ecx, 2\n" /* FAST_FAIL_STACK_COOKIE_CHECK_FAILURE */
	"int $0x29\n"
#else
	/* Somehow, this meaningfully compiles and seemingly matches ABI on arm64. */
	"ret\n"
#endif
);

void __security_init_cookie() {
	/* dummy */
}
