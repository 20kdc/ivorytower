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

* Which `.lib` is which
	* _Note: For various complex reasons, everything goes to `msvcrt.lib` here._
	* <https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-library-features?view=msvc-140>
* `_CRT_INIT` / `mainCRTStartup` / `_DllMainCRTStartup` etc.
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
* `__declspec(thread)` / TLS
	* <https://learn.microsoft.com/en-us/cpp/c-language/thread-local-storage?view=msvc-140>
	* **BIG IMPORTANT SCARY NOTE!** \
	  On at least Windows XP and ReactOS, _static_ TLS indexes are not _dynamic_ TLS indexes.
	  That is, you cannot by-default take a `TlsAlloc()` value and use it as a `_tls_index` value.
	  This explains why, unfortunately, TLS emulation is not possible without changing the LLVM-generated code.
	* Testing shows symbols are:
		* `__dyn_tls_init`
		* `__tlregdtor`
		* `_tls_index`
		* `_tls_array`
	* `_tls_index` / `_tls_used` / `_tls_array`
		* <https://learn.microsoft.com/en-us/windows/win32/debug/pe-format>
			* Indicates `_tls_array` points to `0x2C`/`0x58` (<https://en.wikipedia.org/wiki/Win32_Thread_Information_Block>)
		* <https://github.com/llvm/llvm-project/blob/llvmorg-23.1.4/lld/test/COFF/Inputs/tlssup-32.ll>
	* `_tls_index` / `_tls_array`
		* <https://github.com/llvm/llvm-project/blob/llvmorg-23.1.4/llvm/test/CodeGen/X86/tls.ll#L32>
	* `__tlregdtor`
		* <https://github.com/llvm/llvm-project/blob/llvmorg-23.1.4/clang/lib/CodeGen/MicrosoftCXXABI.cpp#L2413>
	* `__dyn_tls_init` / `.CRT$XDU`
		* <https://github.com/llvm/llvm-project/blob/llvmorg-23.1.4/clang/lib/CodeGen/MicrosoftCXXABI.cpp#L2451>
	* `__tls_guard`
		* <https://github.com/llvm/llvm-project/blob/llvmorg-23.1.4/clang/lib/CodeGen/MicrosoftCXXABI.cpp#L2496>
	* `__dyn_tls_on_demand_init`
		* <https://github.com/llvm/llvm-project/blob/llvmorg-23.1.4/clang/lib/CodeGen/MicrosoftCXXABI.cpp#L2517>
	* `_Init_thread_header` / `_Init_thread_footer` / `_Init_thread_abort`
		* <https://github.com/llvm/llvm-project/blob/llvmorg-23.1.4/clang/lib/CodeGen/MicrosoftCXXABI.cpp#L2599>
	* Basic structure from disasm of `declspec_thread` test:
		* `.tls$` contains actual TLS data.
		* `.CRT$XDU` section has `___tls_init` added.
		* `___tls_init` is a `void __cdecl()`ish function which calls TLS initializers.
		* TLS initializers call `void __cdecl ___tlregdtor(void (*)())`.
* Dllmain
	* <https://learn.microsoft.com/en-us/windows/win32/dlls/dllmain>
	* Key points:
	* 1. If we return FALSE in entrypoint, the system will call DLL_PROCESS_DETACH for us.
	* 2. There's a loader lock, so we can treat this as 'thread-safe-ish'.
	* 3. Destructors are UNSAFE TO RUN unless we are being unloaded dynamically.
	* 4. The CRT entrypoint (*** THIS IS US ***) needs to call C++ constructors/destructors.
* `type_info`:
	* <https://learn.microsoft.com/en-us/windows/win32/memory/stdtypeinfodestroylist>
* 32-bit I64 Libcalls (`__alldiv`/`__aulldiv`/`__allrem`/`__aullrem`/`__allmul`)
	* <https://github.com/llvm/llvm-project/blob/llvmorg-23.1.4/llvm/include/llvm/IR/RuntimeLibcalls.td#L3412>
	* <https://github.com/llvm/llvm-project/blob/llvmorg-23.1.4/llvm/test/CodeGen/X86/win32-int-runtime-libcalls.ll#L26>
