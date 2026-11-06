# Things Tooling Cares About

1. Meson's `cc` value should be as 'pure' as possible, since it uses this for version queries.
2. Meson will 'rewrite' args in various ways.
	* It does not know what `-Xlinker` is. This will break your build if not handled with care.
	* Meson believes link args are _specifically_ for the linker and don't also affect the driver, so it will rewrite them accordingly. This can be a problem in some cases.
3. The `clangd` tool needs to see _anything in the argument list that affects `.c[pp]` -> `.o` compilation._
	* This inherently means Meson will see these args.
4. It is sometimes necessary to 'hide' args from the rest of the tooling using a wrapper binary.
	* Only do this if necessary, it's an ugly hack. If it's for link args then go wild, it won't affect `clangd` UX.
5. `clangd` must be carefully managed. Consider:
	* `x86_64-w64-mingw32-g++-win32`: Works, correctly identified as a MinGW build, can see headers.
	* `/(...)/ivorytower/osxcross_bin/x86_64-apple-darwin20.2-clang++`: _Interesting_ failure.
		* OSXCross is 'hiding' the Clang invocation -- and thus _the OSX sysroot path_ -- inside its wrapper. However, unlike W32Cross, the OSXCross wrapper is doing potentially complex stuff we can't just overwrite.
	* `/(...)/ivorytower/helpers/boxenrunner ivt_docker enter scout-i386 : g++`: Essentially _works by accident._
		* `clangd` _does not know this is a 32-bit platform._
		* It's for this reason that `boxenrunner` was given the ability to inject 'fake args' for clangd.
	* `/(...)/ivorytower/helpers/boxenrunner ivt_docker enter scout-i386 -target i686-linux-gnu : g++`: Mostly works.
		* Obviously `clangd` isn't going to be able to see into the container. However, this setup does treat the host as 'good enough'. It sees the `-target` and is correctly configured.
		* `SDL2/_real_SDL_config.h` error. This is actually a Debian bug. We can't do anything about this without somehow exposing the container's rootfs.
	* `(...)w32cross-clang (...a LOT of args)`: This is basically ideal. `clangd` and `clang` have a (hopefully) obvious mutual understanding (for (hopefully) obvious reasons), so all else being equal any parameter passed to `clang` that works can also be passed to `clangd` and it will work just as well.