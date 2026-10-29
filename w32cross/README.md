# W32Cross

W32Cross is basically intended to be 'like OSXCross, but targetting Windows'.

While it is used in ivorytower for now, it may get split given popular request.

W32Cross is built on four key pillars:

* <https://learn.microsoft.com/en-us/windows/apps/windows-sdk/downloads>: `Windows SDK for Windows 10 2004 (10.0.19041.0)` ISO aka <https://go.microsoft.com/fwlink/?linkid=2312004>
	* Has the useful property of _not being a Visual Studio SDK,_ and thus not subject to 'profit-cap licensing'.
* STL: Can be gotten in code form from <https://github.com/microsoft/STL>.
	* These header files are binary-compatible (or at least _enough_).
* MinGW-w64: The build of compiler-rt, along with some public-domain headers, are taken from here and grafted into the overall whole.
	* See `thirdparty/mingw-w64-headers` for the original headers. Patches are in `notvcrt/ext`.
* `notvcrt`: 'The rest of the owl'. This is the little bit of custom synthetic stuff needed to bridge it all together.

## How It Works

w32cross is run by a series of shell scripts that operate on an 'SDK directory'. It builds in various stages:

1. First, any downloads are handled (defined in i.e. `w10/0sdkdef.sh`)
2. `common/compilerinit.sh` detects the correct names for various tools (i.e. because `clang-cl` is not symlinked to a version on Ubuntu) and builds an 'empty SDK'.
	* The resulting 'empty SDK' contains:
		* `compiler-rt` (but not yet usable)
		* Toolchain arg-lists
		* Wrappers adding said arg-lists to the tools and then running them
		* Many symlinks
		* `w32cross-treecasefix`, which attempts to create lowercase symlinks for all files that might need them
3. The SDK is extracted, rearranged, lowercased, `notvcrt` is built, and then the STL is put in.

## Notes / Caveats

This is tested with `clang` 19 on Debian 13 (`trixie`), but much of the setup was tested also on Ubuntu 22.04.

The replica `vcruntime.h` and friends can be gently described as _sparse._

Binaries appear to work on Windows XP if you memorize [the magic link for the correct redist version](https://download.visualstudio.microsoft.com/download/pr/c168313d-1754-40d4-8928-18632c2e2a71/D305BAA965C9CD1B44EBCD53635EE9ECC6D85B54210E2764C8836F4E9DEFA345/VC_redist.x86.exe). \
(In the given link, the hash after the UUID is a SHA-256 hash, which is useful since it makes it a lot easier to find the other redists in lists.)

### TLS in DLLs

TLS in DLLs (i.e. `__declspec(thread)`) remains unavailable on XP and earlier. Do not use TLS in DLLs. There is a test that uses TLS in DLLs. It crashes on XP. This is an OS limitation.
