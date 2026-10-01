# 'NotVCRuntime': Implementing what's missing

'NotVCRuntime' is a 'fix errors until it works' implementation of:

* _Headers for_ `vcruntime`.
	* `concrt` was considered, but it's not the kind of 'more or less 1:1 mapping' that's reasonably safe to create.
* CRT initialization/shutdown code.
	* VS 2015+ calls this `msvcrt`, although obviously this name meant something different in pre-UCRT times. (The original `msvcrt` is used to this day by MinGW, as it's a reliable cross-vendor source for `malloc`/`free`.)
* Any 'CRT0 VCRuntime' stuff.

It is **not** intended to reimplement VCRuntime itself. If you really need a non-Microsoft version of those DLLs, consider Wine's LGPL vcruntime140 and ucrtbase.

Some headers come from MinGW-w64 (see `../thirdparty/mingw-w64-headers`) and are then patched (see `notvcrt/ext`) for use:
* `eh.h`: Exception handling utility routine header
* `excpt.h`: SEH structures, etc.
* `setjmp.h`: `setjmp`/`longjmp`/`jmp_buf`
* `setjmpex.h`: Fancy MS extension support header

## _When adding files_

Files added in the include directory sometimes go to different places.

There's probably some better organization that can be done here, but for _now_ specifics must be detailed in `w10/4vcruntime.sh`.

## Managing External Header Patches

Patches to MinGW headers are managed through `notvcrt/ext/sync.sh`.

If a patched file does not exist, it creates it; otherwise it syncs changes back to the patches.

This allows the patches to be what's stored in Git while offering an easy development experience.

**The `build.sh` script will wipe these patches.** Invoke `w10/4msvc.sh` directly.

## Useful Reference Material

* <https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-library-features?view=msvc-140>
* <https://learn.microsoft.com/en-us/cpp/build/run-time-library-behavior?view=msvc-140>
* <https://learn.microsoft.com/en-us/cpp/build/reference/entry-entry-point-symbol?view=msvc-140>
* Init/Fini
	* <https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-initialization?view=msvc-140>
	* `Source/10.0.19041.0/ucrt/internal/initialization.cpp`
	* <https://devblogs.microsoft.com/oldnewthing/20141017-00/?p=43823/>
* Atexit behaviour
	* <https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/atexit?view=msvc-170>
* Security cookie
	* <https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/security-init-cookie?view=msvc-140>
* Dllmain
	* <https://learn.microsoft.com/en-us/windows/win32/dlls/dllmain>
	* Key points:
	* 1. If we return FALSE in entrypoint, the system will call DLL_PROCESS_DETACH for us.
	* 2. There's a loader lock, so we can treat this as 'thread-safe-ish'.
	* 3. Destructors are UNSAFE TO RUN unless we are being unloaded dynamically.
	* 4. The CRT entrypoint (*** THIS IS US ***) needs to call C++ constructors/destructors.
* `type_info`:
	* <https://learn.microsoft.com/en-us/windows/win32/memory/stdtypeinfodestroylist>
