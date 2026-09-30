# Why always install the STL?

Other compilers in the `ivorytower` project support STL switch-off.

But not `w32cross`. This is obviously extremely unusual.

The answer is that there really isn't a good way to turn off the STL.

LLVM (and presumably MSVC) only adds so many include directories, and they are:

* Windows SDK (SDK + UCRT) (`WinSdkDir`/`WinSdkVersion` set)
* 'MSVC' (STL + VCRuntime + ATLMFC) (`VCToolsDir`)
	* Note we don't have ATLMFC.

There's no toggle to selectively switch off includes.

In theory, you could intentionally mix `notvcrt`, CRT libraries etc. into i.e. the UCRT regions. (This makes the most logical sense as UCRT headers rely on vcruntime headers.)

This would mean that 'MSVC' only contains STL and ATLMFC, which would allow swapping MSVC directories.

The problem is that `WinSysRoot` overrides `VCToolsDir`.

While this can be done, I don't like the idea of poking the bear of potentially creating layouts that have to change because of a change in representation in LLVM.

I prefer the idea of giving LLVM a layout that matches the SDK era that's being emulated, and then LLVM, which is expected to support this SDK, supporting this SDK because it quacks like the right kind of duck.

With that said, if there's demand for it, or if something comes up (the use of spaces in `Windows Kits` _does_ bother me), the layout can change.

Then there can be `vctoolsdir` and `vctoolsdir_nostl` or something.

Note though that removing `notvcrt` (and thus a proper 'zero' configuration) seems unlikely to impossible. Functions like `_setjmp` and `_longjmpex` appear to go through `vcruntime140`. The question isn't 'can the vcruntime140 dependency be removed', that was foregone due to Clang compatibility.
