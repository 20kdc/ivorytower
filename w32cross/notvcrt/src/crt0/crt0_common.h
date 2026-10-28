/*
 * NOTVCRUNTIME
 * This is not VCRuntime.
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to <http://unlicense.org>, supplied as COPYING in the W32Cross source code.
 */

/*
 * Contains stuff common to all CRT0 files.
 */

#include <corecrt_startup.h>

/* C init */
extern _PIFV __xi_a[1];
extern _PIFV __xi_z[1];
/* C++ init */
extern _PVFV __xc_a[1];
extern _PVFV __xc_z[1];

/* Pre-term */
extern _PVFV __xp_a[1];
extern _PVFV __xp_z[1];
/* Final term */
extern _PVFV __xt_a[1];
extern _PVFV __xt_z[1];

/*
 * crt0 init/finalizer functions.
 * 0 return in __NOTVCRUNTIME_init indicates mysterious failure.
 * Be warned: These functions *MUST* only be called once ever per module, each.
 */
int __NOTVCRUNTIME_init(int isDLL);

/*
 * This only runs for DLLs.
 * EXEs setup everything this *would* do with _crt_atexit in advance.
 * This is based on logic implied by https://devblogs.microsoft.com/oldnewthing/20141017-00/?p=43823/
 */
void __NOTVCRUNTIME_dll_fini();

/* -- LINKER ARG STUFFING AND RATIONALES THEREOF -- */

/*
 * interlocked etc.
 */
#pragma comment(linker, "/defaultlib:kernel32")

/*
 * hack to use 'GNU' compiler-rt with 'MSVC' target
 * this should be literally the only difference
 */
#if defined(__i386__)
#pragma comment(linker, "/alternatename:__chkstk=__alloca")
#elif defined(__x86_64__)
#pragma comment(linker, "/alternatename:__chkstk=___chkstk_ms")
#endif
/*
 * so it is not in fact the only difference.
 * https://github.com/llvm/llvm-project/blob/e9a93baac2f8aad3a2114ec25f6fdb9342a03cd4/llvm/include/llvm/IR/RuntimeLibcalls.td#L3412
 * ugh... to be clear, there is a better way, BUT we can't use it because then Clang tries to link against its personal copy of compiler-rt (which it doesn't have)
 */
#pragma comment(linker, "/alternatename:__alldiv=___divdi3")
#pragma comment(linker, "/alternatename:__aulldiv=___udivdi3")
#pragma comment(linker, "/alternatename:__allrem=___moddi3")
#pragma comment(linker, "/alternatename:__aullrem=___umoddi3")
#pragma comment(linker, "/alternatename:__allmul=___muldi3")

/*
 * I'm not sure why, but Clang is really deathly afraid of emitting "??_7type_info@@6B@".
 * Placing this here forces it to properly link to itself.
 */
#pragma comment(linker, "/alternatename:??_7type_info@@6B@.1=??_7type_info@@6B@")
