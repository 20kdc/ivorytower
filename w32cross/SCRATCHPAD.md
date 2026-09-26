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
