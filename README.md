# ivorytower

`ivorytower` is an attempt at providing something kind of almost _vaguely_ like a mixture of `rustup` and `zig cc` for C++.

It installs a set of crossfiles (presently just for Meson) that are then globally available for use in projects.

## scope

`ivorytower` is a tool to mostly setup 'mostly default' compilation environments.

It is not intended to provide it's own foundational library or other such tools.

However, it may provide an _extremely limited_ C++ `std::` subset for the purposes of enabling 'compiler-bound' language features.

## use of global crossfile caches

ivorytower writes into (presently just Meson)'s global crossfile storage.

This is to provide a 'setup once, use anywhere' experience.

## general toolchain specification layout

A toolchain is specified as a set of four components, separated by `_`:

1. `ivt`. This prefix isolates these specifications from other, non-ivorytower specifications to prevent conflict.
2. _Platform,_ This is a combination of OS and architecture.
	* Templated. This is used for all desktop platforms (and all platforms at present.)
		* OS:
			* `a`: Android
			* `lg`: Linux glibc
			* `w`: Windows (GNU ABI is used)
			* `m`: Mac OS X
		* CPU:
			* `x32`: x86
			* `x64`: x86\_64
			* `av7`: armv7l
			* `a64`: aarch64
3. _C++ STL disposition,_ Among other things, some toolchains have ABI or licensing complexity associated with their C++ standard library.
	* `stl`: The toolchain STL is used in its default mode.
	* `staticstl`: The toolchain STL is used in an explicitly static mode.
		* Remember that you can disable RTTI and exceptions. If your project is multi-module, do this if at all possible.
		* In this mode, you should take care to use _as little of the STL as possible_ if your project is multi-module. `dynamic_cast` is potentially a mismatch hazard.
		* An alternative was considered, but the nature of pure/deleted virtuals meant running a build for every toolchain during SDK install.
	* `none`: The toolchain STL is suppressed. RTTI and exceptions are disabled. There is no replacement.
		* For realistic C++ use, you will need to supply the following functions yourself:
			* `__cxa_pure_virtual`
			* `__cxa_deleted_virtual`
4. _Variant._ Sometimes there are multiple toolchains for a given platform. This is typically used as a proxy to control target OS version.

## the name

This project is named after the proverbial ivory tower, from which all 'just make a build for `$MYPLATFORM`' assumptions sit.
