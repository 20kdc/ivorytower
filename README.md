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
3. _Build disposition,_ Among other things, some toolchains have ABI or licensing complexity associated with their C++ standard library.
	* `stl`: The toolchain STL is used in its default mode.
	* `staticstl`: The toolchain STL is used in an explicitly static mode.
	* `ivt`: The toolchain STL is suppressed. ivorytower's internal 'mini STL' is used.
		* The mini STL, like all of ivorytower, is released into the public domain.
		* The scope of the mini STL is to provide _language feature-relevant_ tools only. In essence, it exists to expose compiler-specific builtins via a standardized interface.
	* `none`: The toolchain STL is suppressed. There is no replacement.
4. _Variant._ Sometimes there are multiple toolchains for a given platform. This is typically used as a proxy to control target OS version.

## the name

This project is named after the proverbial ivory tower, from which all 'just make a build for `$MYPLATFORM`' assumptions sit.
