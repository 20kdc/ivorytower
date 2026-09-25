# ivorytower

![](doc/obligatorylogo.png)

`ivorytower` is an attempt at providing something kind of almost _vaguely_ like a mixture of `rustup` and `zig cc` for C++ from a Linux workstation.

It doesn't take the 'we do it all ourselves' approach of `zig cc` (with the associated problems that brings).

Instead, it intends to provide a unified setup tool for various cross-compilation environments produced by others, namely:

* MinGW-w64: <https://www.mingw-w64.org/>
* `osxcross`: <https://github.com/tpoechtrager/osxcross>
* Valve's 'Steam Runtime' build environment (via `distrobox`): <https://gitlab.steamos.cloud/steamrt/scout/sdk> (etc.)

It, along with these install instructions, results in a set of crossfiles (presently just for Meson) that are then globally available for use in projects.

## how to use it

1. For Windows target support, `x86_64-w64-mingw32-gcc-win32` is required.
2. For Linux target support, `docker` is required. There are two methods here, selected via the `box=` config option on the command line:
	* `box=ivt_docker`: Uses `docker` directly. I've found this is the best experience and thus it's the default.
		* This is implemented in `helpers/boxenrunner`.
		* The environment variable `ITSETUP_BOX_MOUNTS` defaults to `-v /home:/home -v /media:/media` and can be used to adjust exposed mounts.
		* The environment variable `ITSETUP_BOX_ETCFILES` defaults to `-v /etc/passwd:/etc/passwd:ro -v /etc/group:/etc/group:ro` and adjusts exposed etcfiles.
	* `box=distrobox`: Uses `distrobox`.
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

`ivorytower` is a tool to mostly setup 'mostly default' compilation environments for use with Meson.

It is _extremely_ opinionated, but it isn't intended to provide it's own foundational library or other such tools.

## caveats

* Running the build system outside a container and the compiler inside is not, strictly speaking, the fastest way of running a compiler. This will be slower than not doing that.
* Because the Windows builds are based on MinGW, the GNU ABI is used.

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
	* `static`: The toolchain STL is used in an explicitly static mode. `libgcc` is statically linked if necessary.
		* Remember that you can disable RTTI and exceptions. If your project is multi-module, strongly consider doing this.
		* In this mode, you should take care to use _as little of the STL as possible_ at interface boundaries. `dynamic_cast` is potentially a mismatch hazard.
		* An alternative was considered, but the nature of pure/deleted virtuals meant running a build for every toolchain during SDK install.
	* `zero`: C++ STL is suppressed. RTTI and exceptions are disabled. `libgcc` is statically linked if necessary.
		* For realistic C++ use, you will need to supply the following functions yourself:
			* `__cxa_pure_virtual`
			* `__cxa_deleted_virtual`
3. _Variant._ Sometimes there are multiple toolchains for a given platform. This is typically used as a proxy to control target OS version.

Therefore, `ivt/lgx64_default_scout` is SteamRT Scout x86\_64

## the name

This project is named after the proverbial ivory tower, from which all 'just make a build for `$MYPLATFORM`' assumptions sit.
