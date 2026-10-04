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

## Notes

This is tested with `clang` on Ubuntu 24.04.

The replica `vcruntime.h` and friends can be gently described as _sparse._

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
