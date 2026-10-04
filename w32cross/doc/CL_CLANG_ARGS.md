# Regarding translation between `clang-cl` and `clang` args

This is a particularly annoying problem.

The 'reference' is `Clang::AddClangCLArgs` at <https://github.com/llvm/llvm-project/blob/main/clang/lib/Driver/ToolChains/Clang.cpp#L8884>.

Some of these are obvious, but some are not:

* `/GS` becomes `-fstack-protector=strong`.
* `/EH` flags become `-fcxx-exceptions`, `-fexceptions`, and `-fasync-exceptions`.
	* Precision on the parsing lives in `parseClangCLEHFlags`.
		* However, confusingly, `maybeConsumeDash` returns _false_ if there is a dash, and _true_ if there is not. This makes somewhat more sense if understanding that `-` is a negation here, but then you get flags like `NoUnwindC`.
	* Unsurprisingly, asynchronous exceptions enable `-fasync-exceptions`.
		* Elsewhere says that async is 'unimplemented'.
* The pointer-to-member ABI options `/vmg`, `/vmb`, `/vms`, `/vmm`, `/vmv` are footguns and you should realistically never share points-to-member across ABI boundaries on MSVC ABI.
