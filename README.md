# ![](doc/obligatorylogo.png) ivorytower

Compiling C++ across multiple platforms is hard.

Many of those problems are unfixable. But one of those problems is _toolchain configuration,_ and that one is at least fixable for some build systems.

[Meson](https://mesonbuild.com/) is a pretty neat build system. It has a reasonably sensible arrangement and doesn't seem to make too many assumptions about your setup.

But its 'crossfiles' system is obviously dependent on you supplying it with crossfiles.

ivorytower is an attempt at providing something kind of almost _vaguely_ like a mixture of `rustup` and `zig cc` for C++ from a Linux workstation.

It doesn't take the 'we do it all ourselves' approach of `zig cc` (with the associated problems that brings).

Instead, it intends to provide a unified setup tool for various cross-compilation environments produced by others, namely:

* MinGW-w64: <https://www.mingw-w64.org/>
* OSXCross: <https://github.com/tpoechtrager/osxcross>
* Valve's 'Steam Runtime' build environment (via Distrobox): <https://gitlab.steamos.cloud/steamrt/scout/sdk> (etc.)

There is also a custom environment for building MSVC-ABI binaries with the STL, 'W32Cross'. _This option is obviously relatively unstable, but may be useful if its limitations are accepted._

ivorytower, along with these install instructions, results in a set of crossfiles that are then globally available for use in projects.

## how to use it

Start with a Linux system that has at least these things:

* A reasonably sensible `/bin/sh`
* `git`
* For portable Linux support:
	* `docker`
		* Actual Docker is recommended, but there's nothing that _strictly_ requires it (i.e. anything that fails under `podman-docker` is a bug there, not in `ivorytower`, unless you have a very good reason to say otherwise).
* `meson`, with a working host C++ compiler
* A reasonably sensible filesystem layout
	* All source code you intend to compile must be visible from `/home`, `/media`, `$XDG_RUNTIME_DIR` or `/tmp`. Workarounds are possible, see [options](doc/OPTIONS.md)
* For MinGW Windows support:
	* Either/both of `i686-w64-mingw32-gcc-win32` and `x86_64-w64-mingw32-gcc-win32` (also `g++`, etc.)
		* If you want to insist on the 'POSIX threads' compiler, set `mingw_suffix=-posix`.
* For `w32cross` Windows support:
	* `msiextract`, `clang`, `clang++`, `lld-link`, `llvm-lib`, and various other tools.
* For Mac OS X support:
	* `wget`
	* Generally 'a system that OSXCross supports'

Then proceed to clone this repository with the usual `git clone --depth=1`.

ivorytower defaults to setting up the following targets:

* MinGW-w64 (assumed to be provided by your distribution)
	* This target is installed even if the underlying executables do not exist, as the binary names are well-standardized.
* OSXCross (will be downloaded if necessary)
* and SteamRT Scout x86\_64 in a container (via Docker).

Additional targets that can be selected are:

* SteamRT Scout i386 (`scout_i386=on`)
* SteamRT Sniper x86\_64 (`sniper=on`)
* The `w32cross` Clang-MSVC target (`w32cross=on`)
	* This is highly preferrable for various reasons, like interop with existing code, but also must download a Windows SDK ISO (among other things).

## caveats

* For the Linux builds, running the build system outside a container and the compiler inside is not, strictly speaking, the fastest way of running a compiler. This will be slower than not doing that.
* `w32cross` is quasi-stable and MinGW-w64's C++ standard library requires being shipped with your application.

## use of global crossfile caches

ivorytower writes into (presently just Meson)'s global crossfile storage.

This is to provide a 'setup once, use anywhere' experience.

## general toolchain specification layout

A toolchain is specified as the prefix `ivt/`, followed by three components separated by `_`:

1. _Platform,_ This is a combination of OS and architecture.
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
2. _Standard library disposition,_ \
   This covers the C++ STL. On Windows, it also makes `libgcc` and `libatomic` static. \
   Notably, this does _not_ staticize `libc`! That sort of thing either works with `-Dcpp_link_args=-static` or doesn't work at all.
	* `default`: The toolchain STL is used in its default mode, which is usually/always shared library. _This never adds any compiler options._
		* For Linux, use this.
			* I am aware <https://mesonbuild.com/Creating-Linux-binaries.html> says that different Linux distributions have binary-incompatible STLs.
			* Valve uses dynamic STL (insofaras they use the STL at all), and more to the point, shadowing the system STL is a realistic risk **that will break Mesa if it happens.**
			* For as much as GNU library management has caused other issues in this project, `libstdc++` maintainers seem well aware that breaking ABI here would _light everything on fire_ and have evaded that catastrophe.
		* This is the only supported mode for `w32cross`.
	* `static`: The toolchain STL is used in an explicitly static mode. `libgcc` is statically linked if necessary.
		* Remember that you can disable RTTI and exceptions. If your project is multi-module, strongly consider doing this.
		* In this mode, you should take care to use _as little of the STL as possible_ at interface boundaries. `dynamic_cast` is potentially a mismatch hazard.
		* An alternative was considered, but the nature of pure/deleted virtuals meant running a build for every toolchain during SDK install.
	* `zero`: C++ STL is suppressed. RTTI and exceptions are disabled. `libgcc` is statically linked if necessary.
		* For realistic C++ use, you will need to supply the following functions yourself:
			* `__cxa_pure_virtual`
			* `__cxa_deleted_virtual`
3. _Variant._ Sometimes there are multiple toolchains for a given platform. This is typically used as a proxy to control target OS version.

Therefore, `ivt/lgx64_default_scout` is SteamRT Scout x86\_64.

## the name

This project is named after the proverbial ivory tower, from which all 'just make a build for `$MYPLATFORM`' assumptions sit.
