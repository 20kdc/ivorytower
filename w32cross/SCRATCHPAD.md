## Feasibility Test

`clang -fuse-ld=lld-link -target i686-windows-msvc test.cpp -Xlinker /safeseh:no -o test.exe -I /media/ramdisk/msvc600/vc98/include -L /media/ramdisk/msvc600/vc98/Lib`

OK?

## W32Cross 'Fixerupper' C-Only Firing Run

```
#include <stdio.h>
void main() { puts("Hello, world!"); }
```

`/home/t20kdc/Documents/repositories/ivorytower/w32cross/target`

`clear ; clang-cl-18 -fuse-ld=lld-link-18 /winsysroot /home/t20kdc/Documents/repositories/ivorytower/w32cross/target -v a.c` fails, no `vcruntime.h`

Indications it wants an _MSVC_ SDK at `/home/t20kdc/Documents/repositories/ivorytower/w32cross/target/VC/Tools/MSVC/include`

having done this, working on that now

ok! now it's failing because it is trying to link libcmt

`clear ; clang-cl-18 -fuse-ld=lld-link-18 /winsysroot /home/t20kdc/Documents/repositories/ivorytower/w32cross/target /c -v a.c`

`/c` OK got OBJ file

`lld-link-18 /winsysroot:/home/t20kdc/Documents/repositories/ivorytower/w32cross/target /nodefaultlib a.obj`

a few undefined symbols but it looks survivable

adding `ucrt.lib` clears up most of them but not `mainCRTStartup` and `__crt_va_start` and `__crt_va_end`

the VA macros were replaced with emptiness in the hope it's nothing (it probably isn't nothing)

`clear ; clang-cl-18 -fuse-ld=lld-link-18 /winsysroot /home/t20kdc/Documents/repositories/ivorytower/w32cross/target /c -v a.c ; lld-link-18 /winsysroot:/home/t20kdc/Documents/repositories/ivorytower/w32cross/target /nodefaultlib a.obj ucrt.lib`

now only undefined symbol is `mainCRTStartup`

`a.c` changed to:

```
#include <stdio.h>

void main() { puts("Hello, world!"); }

void mainCRTStartup() { main(); }
```

it built and ran

we apparently need to pass `-fms_compatibility_version` or else `va_defs` won't take us seriously

after crt-va enablement

```
#include <stdio.h>

void main() { puts("Hello, world!"); }

void mainCRTStartup() { main(); }

/* there are reports of this being both 32-bit and 64-bit. that makes uintptr_t */
void * __security_cookie;
void __security_check_cookie(void * cookie) {}
```

## analysis of how to get UCRT to autolink

* <https://github.com/llvm/llvm-project/blob/6909c17f14683203621f5a3d8fbf04dfd5d9c623/clang/lib/Driver/ToolChains/MSVC.cpp>
	* contains the _default_ `-defaultlib:` entries and lots of hints as to how to drive the toolchain
	* indicates we can use `-fuse-ld=link` as an oracle to debug MSVC layout
	* implies that `ucrt.lib` needs to be implicitly added. can this be checked with CL? do we simply 'fake it' in wrapper layer?
* <https://github.com/llvm/llvm-project/blob/6909c17f14683203621f5a3d8fbf04dfd5d9c623/llvm/lib/WindowsDriver/MSVCPaths.cpp>
	* part of how `clang-cl` detects MSVC stuff.
	* it's important we query `llvm::useUniversalCRT`. luckily, we know if this is set via `-v` as the `ucrt` includes only appear if this is set.

## Definitive Reference of LLVM Paths By Proof

* <https://github.com/llvm/llvm-project/blob/57630fd809ca4964f887dd531e1a09618d962b10/lld/COFF/Driver.cpp#L839>
	* `UniversalCRTSdkPath = Program Files/Windows Kits/10`
	* `UCRTVersion = 10.0.19041.0`
* <https://github.com/llvm/llvm-project/blob/main/llvm/lib/WindowsDriver/MSVCPaths.cpp#L468C12-L468C33>
	* getUniversalCRTSdkDir **only works using cmdline sysroot on Linux cross**. UCRT is expected to be part of Windows SDK and 'UCRT' variables are 1:1 w/ Windows SDK ones.
* <https://github.com/llvm/llvm-project/blob/main/llvm/lib/WindowsDriver/MSVCPaths.cpp#L99C13-L99C43>
	* `WinSysRoot = Program Files`
	* `WinSdkDir = Program Files/Windows Kits/10`
* <https://github.com/llvm/llvm-project/blob/main/llvm/lib/WindowsDriver/MSVCPaths.cpp#L314>
	* arch append
	* Windows 7 SDK has 32-bit libs in a directory and then 64-bit in `x64`
	* Newer SDKs use `x86`/`x64`/`arm`/`arm64` layout
* <https://github.com/llvm/llvm-project/blob/main/clang/lib/Driver/ToolChains/MSVC.cpp#L696>
	* Note version-sensitive logic, also keep in mind arch append
	* 7: `WindowsSDKLibraryPath = WinSdkDir/Lib/`
	* 8: `WindowsSDKLibraryPath = WinSdkDir/Lib/um/x86`
	* Special logic for 10 but seems to be for versioning only?

## `clang-cl` type\_info oracle for vcruntime\_typeinfo stub

`typeinfo`:

```
namespace std { class type_info { }; }
```

`test.cpp`:

```
#include "typeinfo"
void leak(const std::type_info * other);
class myclass {};
int main() { leak(&typeid(myclass)); return 1; }
```

`clang-cl-18 /c /FA1 test.cpp` spits out an assembly file

```
	lea	rcx, [rip + "??_R0?AVmyclass@@@8"]
	call	"?leak@@YAXPEBVtype_info@std@@@Z"
[...]
"??_R0?AVmyclass@@@8":
	.quad	"??_7type_info@@6B@"
	.quad	0
	.asciz	".?AVmyclass@@"
	.zero	2
```

A public online demangler reports the first quad is the vtable. Therefore an acceptable class layout is:

```
class type_info {
	// (for instance. can be ANY virtual function in theory)
	virtual ~type_info();
};
```

`vcruntime140` symbols suggest that code not implementing vcruntime should forward to the DLL for impls:

```
__std_type_info_compare
__std_type_info_destroy_list
__std_type_info_hash
__std_type_info_name
```

I was kinda concerned about `"??_7type_info@@6B@"` being missing. A random commit message suggests we're actually supposed to supply it in the dynamic vcruntime lib.

## Confirming the creation of a mixed-mode file

```
llvm-lib /machine:amd64 /def:vcruntime140.def /out:vcruntime140.lib
llvm-lib /machine:amd64 vcruntime140.lib test.obj /out:vcruntime.lib
```

This creates a 'mixed' library with both kinds of code. This is necessary to create the vcruntime.lib with `type_info` vtable.
