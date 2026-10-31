# The `clang_args` mess, etc.

Like many parts of ivorytower, this is a mess due to a number of important conflicting concerns.

1. Meson has to actually function, which is difficult when it will rearrange your compiler args as it likes, breaking things like `-Xlinker`.
2. Using the `clang` driver with the MSVC ABI causes it to need to be supplied a lot of args to stop it from making a red bloody mess of the carpet.
	* The least-bad is that `--target` and library and include paths must be specified.
	* `-fms-runtime-lib=dll` will not stop `libcmt` being linked. `-nostartfiles` is mandatory.
	* `--ld-path` appears to be a lie.
	* `--fuse-ld=` only works for executables in PATH.
	* There are some linker args installed here to work around `notvcrt` linker comments not necessarily propagating through.
3. The `clangd` tool needs as many of those args as possible, especially all of the per-architecture stuff from earlier.
	* _That means that Meson needs to see these args._
4. If some of these args should actually secretly be stuffed into the part of the command line that Meson _can't_ see is 'open to interpretation'. But the same thing is basically being done more robustly using the command wrappers already anyway where it's truly needed.

As a result of all of this, a 'layered approach' is used.

1. `bin/w32cross-link`: 'Baseline' command wrappers that simply _add args to your command to work around issues_ (or manipulate i.e. PATH to prepare it for the next layer).
	* This is something of a last resort because the goal here is to trick _the compiler invoking it._
2. `sdk_w10/etc/clang-args` files: These contain all the necessary setup args that go on top of the baseline command wrappers. This is all the stuff we don't need to (and for things like library paths, _really shouldn't_) _hide_ from the build system.
3. `bin/w32cross-clang-x86`: 'User-friendly' command wrappers that add the args from 2.

The `clang-args` files are basically like `compile_flags.txt` except `${W32CROSS_SDKROOT}` needs to be substituted with the SDK root (this only happens for this case).

The order of args is basically a suggestion given how Meson behaves.

## Bad Meson Rewriting

As of Meson 1.7.0, Meson appears to incorrectly rewrite some args into being linker args when it really shouldn't.

This feels like something I would have noticed earlier if this wasn't a regression, but anyway, here's how the compiler wrapper (_not_ the linker wrapper) fixes it:

* `-Wl,-L` is fixed to `-L`
* `-Wl,-nostartfiles` is fixed to `-nostartfiles`

Notably, it seems like this is happening for any arg that's in `c_link_args` but not `c_args` and is _not_ already prefixed with `-Wl,`; this may suggest a misunderstanding on my part or may explain a Meson bug. I don't know which.

The culprit appears to be <https://github.com/mesonbuild/meson/blob/0ef4bf221dcb4d81d05b34801310d212511ada50/mesonbuild/compilers/mixins/clang.py#L243>, which seems to be a well-meaning attempt to ensure that MSVC-style link args actually reach the linker.

And it would have worked, too, if it wasn't for that meddling thing called 'reality' where there are potentially args which the compiler driver (and potentially even build system) do in fact know about which need to go there.

## `clang-cl` and translating between `cl` and `clang`

`clang-cl` requires a lot less prodding, because it knows about things like MSVC CRT variations. But also it's the _original_ reason that `w32cross-link` needed to be a comprehensive wrapper, because it's the _worst_ if you need to tweak anything.

Even worse, if you try to run a `clang-cl` build, Meson insists on driving the linker itself, which inevitably ends in doom. (Especially because then Meson triggers some _very_ sketchy file-path detection logic, which goes about as well as you'd expect.)

Funnily enough, Meson is apparently clever enough to understand when you're using an MSVC-style linker on the backend of a GNU-style compiler, and this works a lot better at Meson-level, which is why `clang-cl` support in `w32cross` remains more of a curiousity.

In regards to translating between `clang-cl` and `clang` args, the 'reference' is [`Clang::AddClangCLArgs`](https://github.com/llvm/llvm-project/blob/main/clang/lib/Driver/ToolChains/Clang.cpp#L8884).

Some of these are obvious, but some are not:

* `/GS` becomes `-fstack-protector=strong`.
* `/EH` flags become `-fcxx-exceptions`, `-fexceptions`, and `-fasync-exceptions`.
	* Precision on the parsing lives in `parseClangCLEHFlags`.
		* However, confusingly, `maybeConsumeDash` returns _false_ if there is a dash, and _true_ if there is not. This makes somewhat more sense if understanding that `-` is a negation here, but then you get flags like `NoUnwindC`.
	* Unsurprisingly, asynchronous exceptions enable `-fasync-exceptions`.
		* Elsewhere says that async is 'unimplemented'.
* The pointer-to-member ABI options `/vmg`, `/vmb`, `/vms`, `/vmm`, `/vmv` are footguns and you should realistically never share points-to-member across ABI boundaries on MSVC ABI.
