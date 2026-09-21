# ivorytower

`ivorytower` is an attempt at providing something kind of almost _vaguely_ like a mixture of `rustup` and `zig cc` for C++ from a Linux workstation.

It doesn't take the 'we do it all ourselves' approach of `zig cc` (with the associated problems that brings).

Instead, it intends to provide a unified setup tool for various cross-compilation environments produced by others, namely:

* MinGW-w64: <https://www.mingw-w64.org/>
* `osxcross`: <https://github.com/tpoechtrager/osxcross>
* Valve's 'Steam Runtime' build environment (via `distrobox`): <https://gitlab.steamos.cloud/steamrt/scout/sdk> (etc.)

It, along with these install instructions, results in a set of crossfiles (presently just for Meson) that are then globally available for use in projects.

## how to use it

1. For Windows target support, `x86_64-w64-mingw32-gcc-win32` is required.
2. For Linux target support, `docker` is required, along with `distrobox`.
	* _Set `DBX_CONTAINER_MANAGER=docker` in your environment!_ `distrobox` likes to prefer `podman`, but it's not actually a good idea to use podman for this.
		* I am considering the merits of simply _making_ `distrobox` use `docker`.
	* **DO NOT USE `podman` FOR THIS.** `podman` will download 800MB then throw it all away because you don't have subuid/subgid setup.
		* Rootlessness isn't even _possible_ here because `distrobox` will run your container `--privileged`. This will cause `podman` to require authentication.
	* **DO NOT USE `lilipod` FOR THIS.** `lilipod` has bad diagnostics; I _think_ this was also the subuid/subgid thing (I tried `podman` after).
3. Install `osxcross` from <https://github.com/tpoechtrager/osxcross>. I went with `stable` tooling.
	* Notably, **`osxcross` is the real cross-compiler here**. ivorytower is meant to be a unified bundling.
	* You have to pick and get an SDK. There are two versions I consider 'worth keeping around'.
		* `MacOSX10.9.tar.xz` contains i386 and x86_64 OSX support. Technically you can use 10.13, but if you care about old hardware compatibility this much you probably want 10.9.
		* `MacOSX11.1.tar.xz` contains x86_64 and aarch64 support.
		* Realistically, unless you want really inflated binaries, you should probably only be picking one of these pairs anyway.

## scope

`ivorytower` is a tool to mostly setup 'mostly default' compilation environments.

It is _extremely_ opinionated, but it isn't intended to provide it's own foundational library or other such tools.

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
		* For Linux, use this.
			* I am aware <https://mesonbuild.com/Creating-Linux-binaries.html> says that different Linux distributions have binary-incompatible STLs.
			* Valve uses dynamic STL (insofaras they use the STL at all), and more to the point, shadowing the system STL is a realistic risk that will break Mesa if it happens.
	* `staticstl`: The toolchain STL is used in an explicitly static mode.
		* Remember that you can disable RTTI and exceptions. If your project is multi-module, strongly consider doing this.
		* In this mode, you should take care to use _as little of the STL as possible_ at interface boundaries. `dynamic_cast` is potentially a mismatch hazard.
		* An alternative was considered, but the nature of pure/deleted virtuals meant running a build for every toolchain during SDK install.
	* `none`: The toolchain STL is suppressed. RTTI and exceptions are disabled. There is no replacement.
		* For realistic C++ use, you will need to supply the following functions yourself:
			* `__cxa_pure_virtual`
			* `__cxa_deleted_virtual`
4. _Variant._ Sometimes there are multiple toolchains for a given platform. This is typically used as a proxy to control target OS version.

## the name

This project is named after the proverbial ivory tower, from which all 'just make a build for `$MYPLATFORM`' assumptions sit.
