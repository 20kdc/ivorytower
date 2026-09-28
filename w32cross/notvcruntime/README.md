# 'NotVCRuntime': Implementing what's missing

'NotVCRuntime' is for a clean 'fix errors until it works' implementation of:

* `vcruntime` and eventually `concrt` headers
	* The goals of the `concrt` headers will primarily be whatever makes STL compilation happy.
	* This does mean it has to partially decode `type_info`. Luckily, `clang-cl` is a `type_info` oracle.
* CRT initialization/shutdown code.
	* VS 2015+ calls this `msvcrt`, although obviously this name meant something different in pre-UCRT times. (The original `msvcrt` is used to this day by MinGW, as it's a reliable cross-vendor source for `malloc`/`free`.)

## Useful Reference Material

* <https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-library-features?view=msvc-140>
* <https://learn.microsoft.com/en-us/cpp/build/run-time-library-behavior?view=msvc-140>
* <https://learn.microsoft.com/en-us/cpp/build/reference/entry-entry-point-symbol?view=msvc-140>
* Init/Fini
	* <https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-initialization?view=msvc-140>
	* `Source/10.0.19041.0/ucrt/internal/initialization.cpp`
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
