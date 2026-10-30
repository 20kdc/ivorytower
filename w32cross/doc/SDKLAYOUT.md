## W32Cross SDK Layout

With the failure of Meson integration under the `clang-cl` approach, it's pretty clear now that our best bet is basically _almost_ to fake being MinGW.

This seems to be the intended approach by Meson, and it does have the key advantage of allowing for a consistent command-line syntax between targets in ivorytower proper, which will likely matter as expansion continues outside of Meson.

* `bin/`: '`PATH`able wrappers' (`w32cross-` prefix)
	* `w32cross-clang[++]`: Sets up environment and runs `clang`. **Does not do any further setup; direct use is 'advanced'.**
	* `w32cross-clang[++]-ARCH`: 'Proper' compiler wrapper. **This sets up everything you actually need to setup to build executables.**
	* `w32cross-link` / `w32cross-lld-link`: Linker wrapper.
	* `w32cross-cl` / `w32cross-clang-cl`: 'Proper' compiler wrapper. More unstable in practice, but doesn't require as much command-line trickery, so no need for per-arch links.
	* `w32cross-cl-ARCH` / `w32cross-clang-cl-ARCH`: All these do is add `--target`.
	* Other tools also wrapped for consistency and possible also options nudging by w32cross.
* `bin_compat/`: friendlier to compiler autodetectors (no `w32cross-` prefix, architecture-named dirs)
* `wbin/ARCH/`: things like `dxc.exe`
* `etc/`: Y'know, stuff.
	* `ready`: Presence indicates readiness.
	* `packages`: List of SDK 'packages'.
	* `meson_bootstrap/`: 'Bootstrap' Meson cross-files used to build NotVCRT.
		* `x86`/`x64`/`arm64` files
		* These files _absolutely_ require `bin/` to be in PATH.
		* These files are left intentionally incomplete. `ivorytower` provides a _proper_ set of Meson crossfiles; these are specifically for bootstrap compilation.
	* `clang-args`: Metadata for setting up Clang correctly without using the wrapper scripts. Has `${W32CROSS_SDKROOT}` substitution.
		* `ARCH.lst`: List for a given architecture. Contains Clang target. Non-linking, does not contain `lib` directories.
		* `ARCH.ld.lst`: List for a given architecture. Contains Clang target and `lib` directories.
* `build/`: Build internal files that can be safely removed.
	* `compiler-rt/`: Contains LLVM `compiler-rt`. The SDK stages merge these into `msvcrt.lib`.
		* `libclang_rt.builtins.ARCH.a`: Pre-mapped into x86\_64
	* `notvcrt_obj/ARCH`: Meson build directories for `notvcrt`.
* `licenses/`: licenses
* `stl/`: STL license, `include/` and `lib/`.
	* `include/`
	* `lib/`
		* `x86`/`x64`/`arm64`
* `ucrt/`: UCRT + NotVCRT (as these depend on each other).
	* `src/`
		* `ucrt/`
		* `notvcrt/`
	* `include/`
	* `lib/`
		* `x86`/`x64`/`arm64`
* `wsdk/`: Merged 'CRT-less' Windows SDK. Should only depend on itself.
	* `include/`
		* `excpt.h` and `eh.h` are moved here as dependencies that shouldn't have been omitted in the first place.
	* `lib/`
		* `x86`/`x64`/`arm64`
* `cppwinrt/`: It's here, I guess.
* `redist/`: Redistributables.
* `fakewinsysroot/`: Makes `clang-cl` work.
* `activate`: Sourcable. Puts `bin/` into PATH.

## Stability Concerns

So, obviously as much as possible _should_ be stable. _But:_

* `fakewinsysroot` is always subject to change
* `wbin` is intentionally left as-is
* `licenses` will contain whatever
* `build` is completely removable

The following paths **must be _absolutely stable_** because `clangd` will work better if the commands are written without wrapping.

* `wsdk/include`, `wsdk/lib/x86`, `wsdk/lib/x64`, `wsdk/lib/arm64`
* `ucrt/include`, `ucrt/lib/x86`, `ucrt/lib/x64`, `ucrt/lib/arm64`
* `stl/include`, `stl/lib/x86`, `stl/lib/x64`, `stl/lib/arm64`

The following paths are the 'external interface' intended for use by enclosures to produce build instructions. If you're seeing this, they're stable.

* `etc/packages`
* `etc/clang-args/*`
	* To be clear, running `clang++` with the args in `x86.ld.lst` followed by i.e. `hello.cpp` should produce a working, ready-to-run (given STL/vcruntime) 32-bit Windows executable.
		* The non-`.ld` files can compile object files but cannot link them. (This is to prevent 'unused' noise.)
		* These files are wrapper equivalents. You can kind of think of this as an 'exo-compiler-driver'.
* `etc/ready`
