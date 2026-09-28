# W32Cross

W32Cross is basically intended to be 'like OSXCross, but targetting Windows'.

While it is used in ivorytower for now, it may get split given popular request.

W32Cross is basically built on two key pillars:

* <https://learn.microsoft.com/en-us/windows/apps/windows-sdk/downloads>: `Windows SDK for Windows 10 2004 (10.0.19041.0)` ISO aka <https://go.microsoft.com/fwlink/?linkid=2312004>
	* Has the useful property of _not being a Visual Studio SDK,_ and thus not subject to 'profit-cap licensing'.
* STL: Can be gotten in code form from <https://github.com/microsoft/STL>.
	* These header files are binary-compatible (or at least _enough_ with).

The main key problem is the lack of the `vcruntime` headers and CRT0, which have to be synthesized.

## Notes

This is tested with `clang-cl-18` on Ubuntu 24.04.

The `/FA1` option appears to produce code which will not assemble in some cases; `vcr_typeinfo.cpp` seems to be this way.

The replica `vcruntime.h` can be gently described as _sparse._
