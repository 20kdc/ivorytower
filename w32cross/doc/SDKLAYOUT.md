## W32Cross SDK Layout

With the failure of Meson integration under the `clang-cl` approach, it's pretty clear now that our best bet is basically _almost_ to fake being MinGW.

This seems to be the intended approach by Meson upstream, and it does have the key advantage of allowing for a consistent command-line syntax between targets in ivorytower proper, which will likely matter as expansion continues outside of Meson.

* `bin/`: '`PATH`able wrappers' (`w32cross-` prefix)
	* `w32cross-clang`: Main compiler wrapper. Does not set target.
	* `w32cross-clang-ARCH`: Compiler wrapper with target set.
	* `w32cross-link` / `w32cross-lld-link`: Linker wrapper
	* Other tools also wrapped for consistency and possible also options nudging by w32cross.
* `bin_compat/`: friendlier to compiler autodetectors (no `w32cross-` prefix, architecture-named dirs)
* `wbin/ARCH/`: things like `dxc.exe`
* `build/`: Build internal files that can be safely removed.
* `licenses/`: licenses
* `stl/`: STL license, `include/` and `lib/`.
	* `include/`
	* `lib/`
* `ucrt/`: UCRT + NotVCRT (as these depend on each other).
	* `src/`
		* `ucrt/`
		* `notvcrt/`
	* `include/`
	* `lib/`
* `wsdk/`: Merged 'CRT-less' Windows SDK. Should only depend on itself.
	* `include/`
		* `excpt.h` and `eh.h` are moved here as dependencies that shouldn't have been omitted in the first place.
	* `lib/`
* `cppwinrt/`: It's here, I guess.
* `redist/`: Redistributables.
